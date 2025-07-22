#pragma once
#include "ResourceManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Shader.hpp"
#include "glm/glm.hpp"
#include <vector>


class UIRenderer {
public:
    UIRenderer();
    void render(const std::vector<size_t>& uiEntities, ECS* ecs);
    void renderText(const std::string& text, const std::string& fontKey, float x, float y, float scale, const glm::vec3& color);

private:
    GLuint textVAO, quadVAO;
    GLuint textVBO, quadVBO, texCoordVBO;
    GLuint textEBO, quadEBO;
    std::shared_ptr<Shader> uiShader;
    std::shared_ptr<Shader> textShader;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    int windowWidth, windowHeight;
    void setupMatrices();
    void initializeQuadBuffers();
    void initializeTextBuffers();
    float calculateTextWidth(const std::string& text, const std::shared_ptr<Font>& font, float scale);
    void drawQuad();
};
