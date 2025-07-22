#include "UIRenderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

UIRenderer::UIRenderer() : windowWidth(1200), windowHeight(800) {
    uiShader = ResourceManager<Shader>::getInstance().get("uiShader");
    textShader = ResourceManager<Shader>::getInstance().get("textShader");

    setupMatrices();
    initializeQuadBuffers();
    initializeTextBuffers();
}

void UIRenderer::setupMatrices() {
    projectionMatrix = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight));
    viewMatrix = glm::mat4(1.0f);
}

void UIRenderer::initializeQuadBuffers() {
    float quadVertices[] = {
        -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 0.0f
    };
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UIRenderer::initializeTextBuffers() {
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UIRenderer::drawQuad() {
    uiShader->use();
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void UIRenderer::render(const std::vector<size_t>& uiEntities, ECS* ecs) {
    glDisable(GL_CULL_FACE);
    for (const auto& entityID : uiEntities) {
        // Retrieve the UITransformComponent for positioning
        auto transform = ecs->getComponent<UITransform>(entityID);
        auto imageComponent = ecs->getComponent<UIImageElement>(entityID);
        auto textComponent = ecs->getComponent<UITextElement>(entityID);

        glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
        modelMatrix = glm::translate(modelMatrix, glm::vec3(transform.position));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.size * transform.scale, 1.0f));

        // Render UIImageComponent if it exists
        
        if (imageComponent.isImageVisible) {
            uiShader->use();
            uiShader->setMat4("projection", projectionMatrix);
            uiShader->setMat4("model", modelMatrix);
            uiShader->setVec4("color", glm::vec4(1.0f)); // Default tint color

            auto texture = ResourceManager<Texture>::getInstance().get(imageComponent.textureCode);
            if (texture) {
                texture->bind(GL_TEXTURE0);
                drawQuad();
            }
        }

        // Render UITextComponent if it exists
        if (textComponent.isTextVisible) {
            
            renderText(
                textComponent.text,
                textComponent.fontCode,
                transform.position.x,
                transform.position.y,
                transform.scale.x,
                textComponent.color
            );
        }
    }
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void UIRenderer::renderText(const std::string& text, const std::string& fontKey, float x, float y, float scale, const glm::vec3& color) {
    
    glDisable(GL_DEPTH_TEST); // Disable depth test for text rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textShader -> use();
    textShader->setVec3("textColor", color);
    textShader->setMat4("projection", projectionMatrix);

    auto fontOpt = ResourceManager<Font>::getInstance().get(fontKey);
    if (!fontOpt) {
        std::cerr << "Font not found for key: " << fontKey << std::endl;
        return;
    }
    auto font = fontOpt;
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);
    
    float initialX = x;
    float lineSpacing = 2.0f;
    
    float textWidth = 0.0f;
    float maxAscent = 0.0f;
    float maxDescent = 0.0f;

    for (const char c : text) {
        if (font->Characters.find(c) == font->Characters.end()) continue;
        const Character& ch = font->Characters.at(c);
        textWidth += (ch.Advance >> 6) * scale;
    }

    // Compute centered X and Y
     x = x - textWidth / 2.0f;
     

    // Render each character
    
    for (const char c : text) {
        if (c == '\n') {
            x = x - textWidth / 2.0f; // Reset X for new line
            y -= font->Characters['H'].Size.y * 1.2f;  // Move down by line height
            continue;
        }
        if (font->Characters.find(c) == font->Characters.end()) continue;

        const Character& ch = font->Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale; // Advance cursor
    }
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST); // Re-enable depth test
}
