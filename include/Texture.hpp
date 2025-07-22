#pragma once
#include <string>
#include <vector>
#include "glad/glad.h"

enum class TextureType {
    TEXTURE_2D,
    CUBEMAP,
    TEXTURE_ARRAY
};

class Texture {
public:
    Texture(TextureType type);
    ~Texture();

    bool loadFromFile(const std::string& filepath);
    void bind(GLenum textureUnit) const;
    GLuint getID() const;

    bool loadTexture(const char* path);
    bool loadCubemap(const std::vector<std::string>& faces);
    bool loadTextureArray(const std::vector<std::string>& filePaths);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

private:
    TextureType textureType;
    GLuint textureID;
    void configureParameters();
};