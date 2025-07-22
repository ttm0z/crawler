#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>  // OpenGL loader, adjust as needed


class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, bool isSkinned);
    ~Mesh();
    void draw(GLuint shaderProgram) const;
    std::vector<float> getVertices() const;
    std::vector<unsigned int> getIndices() const;
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    glm::vec4 baseColor = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);
    void setBaseColor(glm::vec4 color);

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
    
    void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void setupStaticMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};

#endif // MESH_HPP
