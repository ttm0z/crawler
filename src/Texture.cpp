#include "Texture.hpp"
#include <cmath>
#include <stb_image.h>
#include <iostream>

Texture::Texture(TextureType type) : textureType(type), textureID(0) {
    glGenTextures(1, &textureID);
}

Texture::~Texture() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

bool Texture::loadFromFile(const std::string& filepath) {
    std::cout << "Load 2d texture\n";
    if (textureType != TextureType::TEXTURE_2D) {
        std::cerr << "Error: Wrong texture type. Expected 2D texture." << std::endl;
        return false;
    }

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return false;
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    configureParameters();
    stbi_image_free(data);
    return true;
}

bool Texture::loadCubemap(const std::vector<std::string>& faces) {
    if (textureType != TextureType::CUBEMAP) {
        std::cerr << "Error: Wrong texture type. Expected Cubemap." << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, channels;

    for (size_t i = 0; i < faces.size(); ++i) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load cubemap face: " << faces[i] << std::endl;
            return false;
        }

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    configureParameters();
    return true;
}

bool Texture::loadTextureArray(const std::vector<std::string>& layers) {
    std::cout << "Loading texture array...\n";

    if (textureType != TextureType::TEXTURE_ARRAY) {
        std::cerr << "Error: Texture type is not TEXTURE_ARRAY.\n";
        return false;
    }

    if (layers.empty()) {
        std::cerr << "Error: No texture layers provided.\n";
        return false;
    }

    int width, height, channels;

    // Load first image to determine size and format
    unsigned char* firstData = stbi_load(layers[0].c_str(), &width, &height, &channels, 0);
    if (!firstData) {
        std::cerr << "Failed to load texture array layer: " << layers[0] << std::endl;
        return false;
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;

    int mipLevels = static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1;

    // Generate and bind texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

    // Allocate storage with mipmaps
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, internalFormat, width, height, layers.size());

    // Upload first layer
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, format, GL_UNSIGNED_BYTE, firstData);
    stbi_image_free(firstData);

    // Upload remaining layers
    for (size_t i = 1; i < layers.size(); ++i) {
        int w, h, ch;
        unsigned char* data = stbi_load(layers[i].c_str(), &w, &h, &ch, 0);
        if (!data || w != width || h != height || ch != channels) {
            std::cerr << "Failed to load or mismatch in layer: " << layers[i] << std::endl;
            stbi_image_free(data);
            return false;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    // Set filtering and wrap parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generate mipmaps once
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    // Unbind
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    std::cout << "Texture array loaded successfully with " << mipLevels << " mip levels.\n";
    return true;
}



void Texture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    switch (textureType) {
        case TextureType::TEXTURE_2D:
            glBindTexture(GL_TEXTURE_2D, textureID);
            break;
        case TextureType::CUBEMAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
            break;
        case TextureType::TEXTURE_ARRAY:
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
            break;
    }
}


GLuint Texture::getID() const {
    return textureID;
}

void Texture::configureParameters() {
    std::cout << "configureParameters\n";
    switch (textureType) {
        case TextureType::TEXTURE_2D:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        case TextureType::TEXTURE_ARRAY:
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TextureType::CUBEMAP:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            break;
    }
}
