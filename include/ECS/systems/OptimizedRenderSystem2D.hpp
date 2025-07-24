#pragma once
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "../System.hpp"
#include "../Components.hpp"
#include "../Archetypes.hpp"
#include "../../ResourceManager.hpp"
#include "../../SpriteBatcher.hpp"
#include "../../TextureAtlas.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class OptimizedRenderSystem2D : public System {
public:
    size_t cameraEntity;
    bool enableBatching = true;
    bool enableFrustumCulling = true;
    bool showDebugInfo = false;

    OptimizedRenderSystem2D() {
        setSignature({
            typeid(SpriteComponent).hash_code(),
            typeid(TransformComponent2D).hash_code()
        });
    }

    void init() {
        // Initialize sprite render manager
        SpriteRenderManager::getInstance().init();
        
        // Set up default atlas
        auto& atlasManager = TextureAtlasManager::getInstance();
        defaultAtlas = atlasManager.createAtlas("default", 1024, 1024);
        
        std::cout << "OptimizedRenderSystem2D initialized with batching support" << std::endl;
    }

    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        
        // Get 2D camera
        auto& camera = ecs.getComponent<CameraComponent2D>(cameraEntity);
        
        // Update frustum culling bounds
        if (enableFrustumCulling) {
            updateFrustumCulling(camera);
        }
        
        if (enableBatching) {
            renderWithBatching(entities, ecs, camera);
        } else {
            renderLegacy(entities, ecs, camera);
        }
        
        // Display debug information
        if (showDebugInfo) {
            displayDebugInfo();
        }
    }
    
    // Load sprites into atlas for batching
    bool loadSpriteToAtlas(const std::string& spriteName, const std::string& filePath, 
                          const std::string& atlasName = "default") {
        return TextureAtlasManager::getInstance().loadSpriteToAtlas(atlasName, spriteName, filePath);
    }
    
    // Generate all atlases (call after loading all sprites)
    void generateAtlases() {
        TextureAtlasManager::getInstance().generateAllAtlases();
        std::cout << "All texture atlases generated" << std::endl;
    }
    
    // Create sprite entity with atlas support
    size_t createSpriteEntity(ECS& ecs, const std::string& spriteName, const std::string& atlasName,
                             const glm::vec3& position, const glm::vec2& scale = glm::vec2(1.0f),
                             const glm::vec4& color = glm::vec4(1.0f), int layer = 0) {
        size_t entity = ecs.createEntity();
        
        // Add transform component
        ecs.addComponent(entity, TransformComponent2D(position, glm::vec2(0.0f), scale));
        
        // Add sprite component with atlas info
        ecs.addComponent(entity, SpriteComponent(spriteName, atlasName, color, layer));
        
        return entity;
    }
    
    // Performance monitoring
    void toggleDebugInfo() { showDebugInfo = !showDebugInfo; }
    void toggleBatching() { enableBatching = !enableBatching; }
    void toggleFrustumCulling() { enableFrustumCulling = !enableFrustumCulling; }

private:
    std::shared_ptr<TextureAtlas> defaultAtlas;
    
    void renderWithBatching(const std::vector<size_t>& entities, ECS& ecs, const CameraComponent2D& camera) {
        auto& renderManager = SpriteRenderManager::getInstance();
        
        // Begin batched rendering frame
        glm::mat4 viewProjection = camera.projectionMatrix * camera.viewMatrix;
        renderManager.beginFrame(viewProjection);
        
        // Add all sprites to batch
        for (auto entity : entities) {
            auto& sprite = ecs.getComponent<SpriteComponent>(entity);
            auto& transform = ecs.getComponent<TransformComponent2D>(entity);
            
            if (!sprite.useBatching) {
                // Render immediately for sprites that don't use batching
                renderSpriteImmediate(sprite, transform, camera);
                continue;
            }
            
            // Create transform matrix
            glm::mat4 model = createTransformMatrix(transform);
            
            if (!sprite.spriteName.empty() && !sprite.atlasName.empty()) {
                // Render atlas-based sprite
                renderManager.renderSprite(sprite.spriteName, model, sprite.color, sprite.renderLayer);
            } else if (sprite.textureID > 0) {
                // Render direct texture sprite
                glm::vec2 uvMin = sprite.textureOffset;
                glm::vec2 uvMax = sprite.textureOffset + sprite.textureSize;
                
                // Handle flipping
                if (sprite.flipX) {
                    std::swap(uvMin.x, uvMax.x);
                }
                if (sprite.flipY) {
                    std::swap(uvMin.y, uvMax.y);
                }
                
                renderManager.renderSprite(sprite.textureID, model, sprite.color, uvMin, uvMax, sprite.renderLayer);
            }
        }
        
        // End batched rendering (submits all draw calls)
        renderManager.endFrame();
    }
    
    void renderLegacy(const std::vector<size_t>& entities, ECS& ecs, const CameraComponent2D& camera) {
        // Fallback to original immediate-mode rendering
        for (auto entity : entities) {
            auto& sprite = ecs.getComponent<SpriteComponent>(entity);
            auto& transform = ecs.getComponent<TransformComponent2D>(entity);
            
            renderSpriteImmediate(sprite, transform, camera);
        }
    }
    
    void renderSpriteImmediate(const SpriteComponent& sprite, const TransformComponent2D& transform, 
                              const CameraComponent2D& camera) {
        if (!sprite.shader || (sprite.textureID == 0 && sprite.spriteName.empty())) {
            return;
        }
        
        GLuint textureToUse = sprite.textureID;
        glm::vec2 uvMin = sprite.textureOffset;
        glm::vec2 uvMax = sprite.textureOffset + sprite.textureSize;
        
        // Handle atlas sprites
        if (!sprite.spriteName.empty() && !sprite.atlasName.empty()) {
            auto atlas = TextureAtlasManager::getInstance().getAtlas(sprite.atlasName);
            if (atlas) {
                const SpriteUV* spriteUV = atlas->getSpriteUV(sprite.spriteName);
                if (spriteUV) {
                    textureToUse = atlas->getTextureID();
                    uvMin = spriteUV->uv0;
                    uvMax = spriteUV->uv1;
                }
            }
        }
        
        if (textureToUse == 0) return;
        
        glUseProgram(sprite.shader->ID);
        
        // Set up matrices
        glm::mat4 model = createTransformMatrix(transform);
        
        // Upload matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(sprite.shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(sprite.shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(sprite.shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix));
        
        // Upload sprite color
        glUniform4fv(glGetUniformLocation(sprite.shader->ID, "spriteColor"), 1, glm::value_ptr(sprite.color));
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureToUse);
        glUniform1i(glGetUniformLocation(sprite.shader->ID, "sprite"), 0);
        
        // Render quad
        renderQuad();
    }
    
    glm::mat4 createTransformMatrix(const TransformComponent2D& transform) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, glm::vec3(transform.scale, 1.0f));
        return model;
    }
    
    void updateFrustumCulling(const CameraComponent2D& camera) {
        auto& renderManager = SpriteRenderManager::getInstance();
        renderManager.setFrustumCullingEnabled(enableFrustumCulling);
        
        if (enableFrustumCulling) {
            // Calculate view bounds from camera
            glm::vec2 cameraPos = camera.position;
            
            // Estimate view size from projection matrix
            float orthoWidth = 2.0f / camera.projectionMatrix[0][0];
            float orthoHeight = 2.0f / camera.projectionMatrix[1][1];
            glm::vec2 viewSize(orthoWidth, orthoHeight);
            
            renderManager.updateFrustum(cameraPos, viewSize, camera.zoom);
        }
    }
    
    void displayDebugInfo() {
        const auto& stats = SpriteRenderManager::getInstance().getStats();
        
        std::cout << "=== Render Stats ===" << std::endl;
        std::cout << "Draw Calls: " << stats.drawCalls << std::endl;
        std::cout << "Sprites Rendered: " << stats.spritesRendered << std::endl;
        std::cout << "Sprites Culled: " << stats.spritesCulled << std::endl;
        std::cout << "Batches Created: " << stats.batchesCreated << std::endl;
        std::cout << "Frame Time: " << stats.lastFrameTime << "ms" << std::endl;
        std::cout << "===================" << std::endl;
    }
    
    void renderQuad() {
        // Static quad for immediate rendering (fallback)
        static GLuint quadVAO = 0;
        static GLuint quadVBO = 0;
        
        if (quadVAO == 0) {
            // Set up quad vertices
            float quadVertices[] = {
                // positions   // texture coords
                -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.0f, 1.0f
            };
            
            GLuint indices[] = {
                0, 1, 2,
                2, 3, 0
            };
            
            GLuint quadEBO;
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glGenBuffers(1, &quadEBO);
            
            glBindVertexArray(quadVAO);
            
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            
            // Position attribute
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            
            // Texture coordinate attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);
            
            glBindVertexArray(0);
        }
        
        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};