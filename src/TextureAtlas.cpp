#include "TextureAtlas.hpp"
#include "stb_image.h"
#include <algorithm>
#include <iostream>
#include <cstring>

TextureAtlas::TextureAtlas(int width, int height) 
    : textureID(0), atlasWidth(width), atlasHeight(height), isGenerated(false) {
    rootNode = std::make_unique<AtlasNode>(0, 0, width, height);
    
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

TextureAtlas::~TextureAtlas() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

bool TextureAtlas::addSprite(const std::string& spriteName, const std::string& filePath) {
    if (isGenerated) {
        std::cerr << "Cannot add sprite to already generated atlas: " << spriteName << std::endl;
        return false;
    }
    
    if (spriteDataMap.find(spriteName) != spriteDataMap.end()) {
        std::cerr << "Sprite already exists in atlas: " << spriteName << std::endl;
        return false;
    }
    
    auto spriteData = std::make_unique<SpriteData>();
    spriteData->data = stbi_load(filePath.c_str(), &spriteData->width, &spriteData->height, &spriteData->channels, 4); // Force RGBA
    spriteData->allocated = true;
    
    if (!spriteData->data) {
        std::cerr << "Failed to load sprite: " << filePath << std::endl;
        return false;
    }
    
    // Store channels as 4 since we forced RGBA
    spriteData->channels = 4;
    
    spriteDataMap[spriteName] = std::move(spriteData);
    return true;
}

bool TextureAtlas::addSprite(const std::string& spriteName, unsigned char* data, int width, int height, int channels) {
    if (isGenerated) {
        std::cerr << "Cannot add sprite to already generated atlas: " << spriteName << std::endl;
        return false;
    }
    
    if (spriteDataMap.find(spriteName) != spriteDataMap.end()) {
        std::cerr << "Sprite already exists in atlas: " << spriteName << std::endl;
        return false;
    }
    
    auto spriteData = std::make_unique<SpriteData>();
    spriteData->data = data;
    spriteData->width = width;
    spriteData->height = height;
    spriteData->channels = channels;
    spriteData->allocated = false; // Caller manages memory
    
    spriteDataMap[spriteName] = std::move(spriteData);
    return true;
}

const SpriteUV* TextureAtlas::getSpriteUV(const std::string& spriteName) const {
    auto it = spriteUVs.find(spriteName);
    return (it != spriteUVs.end()) ? &it->second : nullptr;
}

bool TextureAtlas::generateAtlas() {
    if (isGenerated) return true;
    
    if (!packSprites()) {
        std::cerr << "Failed to pack sprites into atlas" << std::endl;
        return false;
    }
    
    generateTexture();
    isGenerated = true;
    
    // Clear sprite data after generating texture to save memory
    spriteDataMap.clear();
    
    return true;
}

std::vector<std::string> TextureAtlas::getSpriteNames() const {
    std::vector<std::string> names;
    for (const auto& pair : spriteUVs) {
        names.push_back(pair.first);
    }
    return names;
}

TextureAtlas::AtlasNode* TextureAtlas::insertSprite(AtlasNode* node, int width, int height) {
    if (!node) return nullptr;
    
    // If we have children, try to insert in them
    if (node->left && node->right) {
        AtlasNode* result = insertSprite(node->left.get(), width, height);
        if (result) return result;
        return insertSprite(node->right.get(), width, height);
    }
    
    // If this node is occupied, can't use it
    if (node->occupied) return nullptr;
    
    // If sprite doesn't fit, can't use this node
    if (width > node->width || height > node->height) return nullptr;
    
    // If sprite fits exactly, use this node
    if (width == node->width && height == node->height) {
        node->occupied = true;
        return node;
    }
    
    // Split the node
    int dw = node->width - width;
    int dh = node->height - height;
    
    if (dw > dh) {
        // Split vertically
        node->left = std::make_unique<AtlasNode>(node->x, node->y, width, node->height);
        node->right = std::make_unique<AtlasNode>(node->x + width, node->y, node->width - width, node->height);
    } else {
        // Split horizontally  
        node->left = std::make_unique<AtlasNode>(node->x, node->y, node->width, height);
        node->right = std::make_unique<AtlasNode>(node->x, node->y + height, node->width, node->height - height);
    }
    
    return insertSprite(node->left.get(), width, height);
}

bool TextureAtlas::packSprites() {
    // Sort sprites by area (largest first) for better packing
    std::vector<std::pair<std::string, SpriteData*>> sortedSprites;
    for (auto& pair : spriteDataMap) {
        sortedSprites.emplace_back(pair.first, pair.second.get());
    }
    
    std::sort(sortedSprites.begin(), sortedSprites.end(),
        [](const auto& a, const auto& b) {
            return (a.second->width * a.second->height) > (b.second->width * b.second->height);
        });
    
    // Pack each sprite
    for (const auto& pair : sortedSprites) {
        const std::string& name = pair.first;
        const SpriteData* sprite = pair.second;
        
        // Add 1 pixel border to prevent bleeding
        int paddedWidth = sprite->width + 2;
        int paddedHeight = sprite->height + 2;
        
        AtlasNode* node = insertSprite(rootNode.get(), paddedWidth, paddedHeight);
        if (!node) {
            std::cerr << "Failed to pack sprite: " << name << " (size: " << sprite->width << "x" << sprite->height << ")" << std::endl;
            return false;
        }
        
        // Calculate UV coordinates (accounting for 1-pixel border)
        float u0 = (float)(node->x + 1) / atlasWidth;
        float v0 = (float)(node->y + 1) / atlasHeight;
        float u1 = (float)(node->x + 1 + sprite->width) / atlasWidth;
        float v1 = (float)(node->y + 1 + sprite->height) / atlasHeight;
        
        spriteUVs[name] = SpriteUV(
            glm::vec2(u0, v0),
            glm::vec2(u1, v1),
            glm::vec2(sprite->width, sprite->height)
        );
    }
    
    return true;
}

void TextureAtlas::generateTexture() {
    // Create empty atlas texture
    std::vector<unsigned char> atlasData(atlasWidth * atlasHeight * 4, 0);
    
    // Copy sprite data to atlas
    for (const auto& pair : spriteDataMap) {
        const std::string& name = pair.first;
        const SpriteData* sprite = pair.second.get();
        const SpriteUV& uv = spriteUVs[name];
        
        // Calculate pixel positions from UV coordinates  
        int x = (int)(uv.uv0.x * atlasWidth);
        int y = (int)(uv.uv0.y * atlasHeight);
        
        // Copy sprite pixels to atlas
        for (int row = 0; row < sprite->height; ++row) {
            for (int col = 0; col < sprite->width; ++col) {
                int srcIndex = ((sprite->height - 1 - row) * sprite->width + col) * sprite->channels;
                int dstIndex = ((y + row) * atlasWidth + (x + col)) * 4;
                
                if (dstIndex + 3 < atlasData.size() && srcIndex + 3 < sprite->width * sprite->height * sprite->channels) {
                    atlasData[dstIndex + 0] = sprite->data[srcIndex + 0]; // R
                    atlasData[dstIndex + 1] = sprite->data[srcIndex + 1]; // G  
                    atlasData[dstIndex + 2] = sprite->data[srcIndex + 2]; // B
                    atlasData[dstIndex + 3] = (sprite->channels == 4) ? sprite->data[srcIndex + 3] : 255; // A
                }
            }
        }
    }
    
    // Upload to GPU
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

// TextureAtlasManager implementation
TextureAtlasManager& TextureAtlasManager::getInstance() {
    static TextureAtlasManager instance;
    return instance;
}

std::shared_ptr<TextureAtlas> TextureAtlasManager::createAtlas(const std::string& name, int width, int height) {
    auto atlas = std::make_shared<TextureAtlas>(width, height);
    atlases[name] = atlas;
    return atlas;
}

std::shared_ptr<TextureAtlas> TextureAtlasManager::getAtlas(const std::string& name) {
    auto it = atlases.find(name);
    return (it != atlases.end()) ? it->second : nullptr;
}

bool TextureAtlasManager::loadSpriteToAtlas(const std::string& atlasName, const std::string& spriteName, const std::string& filePath) {
    auto atlas = getAtlas(atlasName);
    if (!atlas) {
        std::cerr << "Atlas not found: " << atlasName << std::endl;
        return false;
    }
    
    bool result = atlas->addSprite(spriteName, filePath);
    if (result) {
        spriteToAtlasMap[spriteName] = atlasName;
    }
    return result;
}

std::shared_ptr<TextureAtlas> TextureAtlasManager::findAtlasForSprite(const std::string& spriteName) {
    auto it = spriteToAtlasMap.find(spriteName);
    if (it != spriteToAtlasMap.end()) {
        return getAtlas(it->second);
    }
    return nullptr;
}

void TextureAtlasManager::generateAllAtlases() {
    for (auto& pair : atlases) {
        pair.second->generateAtlas();
    }
}