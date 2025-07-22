#ifndef SHADER_H
#define SHADER_H
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
    unsigned int ID;

    // Default constructor
    Shader();

    // Load shader from file paths
    bool loadFromFile(const char* vertexPath, const char* fragmentPath);

    // Activate the shader
    void use() const;

    // Utility uniform functions (setting uniforms)
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // Compile and link shaders
    bool compileAndLink(const char* vertexCode, const char* fragmentCode);
    
    // Utility function for checking shader compilation/linking errors.
    bool checkCompileErrors(GLuint shader, const std::string& type);
};

#endif