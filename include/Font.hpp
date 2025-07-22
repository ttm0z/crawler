#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>
#include <iostream>
#include <ResourceManager.hpp>
#include <glm/glm.hpp>
#include "Shader.hpp"

// Represents a single character glyph
struct Character {
    unsigned int TextureID; // Texture ID of the glyph
    glm::ivec2   Size;      // Size of the glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Offset to advance to next glyph
};

class Font {
public:
    Font();
    ~Font();
    std::map<char, Character> Characters;
    unsigned int VAO;
    unsigned int VBO;
    // Loads a font from a file with the specified size
    Shader m_tileShader;
    bool loadFromFile(const std::string& fontPath, unsigned int fontSize);
    bool renderText(std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif // FONT_HPP
