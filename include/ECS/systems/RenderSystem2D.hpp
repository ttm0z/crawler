#pragma once
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "../System.hpp"
#include "../Components.hpp"
#include "../Archetypes.hpp"
#include "../../ResourceManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderSystem2D : public System {
public:
    size_t cameraEntity;

    RenderSystem2D() {
        setSignature({
            typeid(SpriteComponent).hash_code(),
            typeid(TransformComponent2D).hash_code()
        });
    }

    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        
        // Get 2D camera
        auto& camera = ecs.getComponent<CameraComponent2D>(cameraEntity);
        
        // Simple 2D sprite rendering
        for (auto entity : entities) {
            auto& sprite = ecs.getComponent<SpriteComponent>(entity);
            auto& transform = ecs.getComponent<TransformComponent2D>(entity);
            
            if (sprite.shader && sprite.textureID > 0) {
                renderSprite(sprite, transform, camera);
            }
        }
    }

private:
    void renderSprite(const SpriteComponent& sprite, const TransformComponent2D& transform, const CameraComponent2D& camera) {
        glUseProgram(sprite.shader->ID);
        
        // Set up matrices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, glm::vec3(transform.scale, 1.0f));
        
        // Upload matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(sprite.shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(sprite.shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(sprite.shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix));
        
        // Upload sprite color
        glUniform4fv(glGetUniformLocation(sprite.shader->ID, "spriteColor"), 1, glm::value_ptr(sprite.color));
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite.textureID);
        glUniform1i(glGetUniformLocation(sprite.shader->ID, "sprite"), 0);
        
        // Render quad (we'll need to set up a simple quad VBO/VAO)
        // For now, this is a placeholder
        renderQuad();
    }
    
    void renderQuad() {
        // Simple quad rendering - would need to be implemented
        // with a static VAO/VBO for a unit quad
    }
};