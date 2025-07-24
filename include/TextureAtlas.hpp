#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

struct SpriteUV {
    glm::vec2 uv0;  // Bottom-left UV
    glm::vec2 uv1;  // Top-right UV
    glm::vec2 size; // Original size in pixels (for scaling)
    
    SpriteUV(glm::vec2 bottomLeft = glm::vec2(0.0f), 
             glm::vec2 topRight = glm::vec2(1.0f),
             glm::vec2 pixelSize = glm::vec2(64.0f))
        : uv0(bottomLeft), uv1(topRight), size(pixelSize) {}
};

class TextureAtlas {
public:
    TextureAtlas(int width = 1024, int height = 1024);
    ~TextureAtlas();
    
    // Add a sprite from file to the atlas
    bool addSprite(const std::string& spriteName, const std::string& filePath);
    
    // Add a sprite from existing texture data
    bool addSprite(const std::string& spriteName, unsigned char* data, int width, int height, int channels);
    
    // Get UV coordinates for a sprite
    const SpriteUV* getSpriteUV(const std::string& spriteName) const;
    
    // Get the atlas texture ID
    GLuint getTextureID() const { return textureID; }
    
    // Get atlas dimensions
    int getWidth() const { return atlasWidth; }
    int getHeight() const { return atlasHeight; }
    
    // Generate the atlas texture (call after adding all sprites)
    bool generateAtlas();
    
    // Get all sprite names (for debugging/iteration)
    std::vector<std::string> getSpriteNames() const;
    
private:
    struct SpriteData {
        unsigned char* data;
        int width, height, channels;
        bool allocated; // Track if we need to free the data
        
        SpriteData() : data(nullptr), width(0), height(0), channels(0), allocated(false) {}
        ~SpriteData() { if (allocated && data) delete[] data; }
    };
    
    struct AtlasNode {
        int x, y, width, height;
        bool occupied;
        std::unique_ptr<AtlasNode> left, right;
        
        AtlasNode(int x, int y, int w, int h) 
            : x(x), y(y), width(w), height(h), occupied(false), left(nullptr), right(nullptr) {}
    };
    
    GLuint textureID;
    int atlasWidth, atlasHeight;
    std::unordered_map<std::string, SpriteUV> spriteUVs;
    std::unordered_map<std::string, std::unique_ptr<SpriteData>> spriteDataMap;
    std::unique_ptr<AtlasNode> rootNode;
    bool isGenerated;
    
    // Texture packing algorithm
    AtlasNode* insertSprite(AtlasNode* node, int width, int height);
    
    // Pack all sprites into the atlas
    bool packSprites();
    
    // Generate the final texture
    void generateTexture();
};

class TextureAtlasManager {
public:
    static TextureAtlasManager& getInstance();
    
    // Create a new atlas
    std::shared_ptr<TextureAtlas> createAtlas(const std::string& name, int width = 1024, int height = 1024);
    
    // Get an existing atlas
    std::shared_ptr<TextureAtlas> getAtlas(const std::string& name);
    
    // Load a sprite into a specific atlas
    bool loadSpriteToAtlas(const std::string& atlasName, const std::string& spriteName, const std::string& filePath);
    
    // Find which atlas contains a sprite
    std::shared_ptr<TextureAtlas> findAtlasForSprite(const std::string& spriteName);
    
    // Generate all atlases
    void generateAllAtlases();
    
private:
    std::unordered_map<std::string, std::shared_ptr<TextureAtlas>> atlases;
    std::unordered_map<std::string, std::string> spriteToAtlasMap; // sprite name -> atlas name
    
    TextureAtlasManager() = default;
};