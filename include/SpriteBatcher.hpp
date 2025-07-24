#pragma once
#include "glad/glad.h"
#include "TextureAtlas.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

struct SpriteVertex {
    glm::vec3 position;     // World position
    glm::vec2 texCoord;     // UV coordinates  
    glm::vec4 color;        // Tint color
    float textureIndex;     // Which texture to sample from
    
    SpriteVertex(glm::vec3 pos = glm::vec3(0.0f), 
                 glm::vec2 uv = glm::vec2(0.0f),
                 glm::vec4 col = glm::vec4(1.0f),
                 float texIdx = 0.0f)
        : position(pos), texCoord(uv), color(col), textureIndex(texIdx) {}
};

struct SpriteRenderCommand {
    glm::mat4 transform;
    glm::vec4 color;
    glm::vec2 uvMin, uvMax;
    float textureIndex;
    int layer;
    
    SpriteRenderCommand() 
        : transform(1.0f), color(1.0f), uvMin(0.0f), uvMax(1.0f), textureIndex(0.0f), layer(0) {}
};

struct SpriteBatch {
    std::vector<SpriteRenderCommand> commands;
    std::vector<GLuint> textureIDs;
    int maxTextures;
    
    SpriteBatch(int maxTex = 8) : maxTextures(maxTex) {
        textureIDs.reserve(maxTextures);
        commands.reserve(1000); // Pre-allocate for performance
    }
    
    void clear() {
        commands.clear();
        textureIDs.clear();
    }
    
    bool canAddTexture(GLuint textureID) const {
        if (textureIDs.size() >= maxTextures) {
            // Check if texture is already in batch
            for (GLuint id : textureIDs) {
                if (id == textureID) return true;
            }
            return false;
        }
        return true;
    }
    
    float getTextureIndex(GLuint textureID) {
        // Find existing texture
        for (size_t i = 0; i < textureIDs.size(); ++i) {
            if (textureIDs[i] == textureID) {
                return static_cast<float>(i);
            }
        }
        
        // Add new texture if there's space
        if (textureIDs.size() < maxTextures) {
            textureIDs.push_back(textureID);
            return static_cast<float>(textureIDs.size() - 1);
        }
        
        return 0.0f; // Fallback
    }
};

class SpriteBatcher {
public:
    SpriteBatcher(int maxSpritesPerBatch = 1000, int maxTexturesPerBatch = 8);
    ~SpriteBatcher();
    
    // Initialize OpenGL resources
    void init();
    
    // Begin batching frame
    void begin(const glm::mat4& viewProjectionMatrix);
    
    // Add a sprite to be rendered
    void addSprite(const glm::mat4& transform, 
                   const glm::vec4& color,
                   GLuint textureID,
                   const glm::vec2& uvMin = glm::vec2(0.0f, 0.0f),
                   const glm::vec2& uvMax = glm::vec2(1.0f, 1.0f),
                   int layer = 0);
    
    // End batching and submit all draw calls
    void end();
    
    // Set up frustum culling bounds
    void setFrustumBounds(const glm::vec2& min, const glm::vec2& max, const glm::vec2& size);
    
    // Enable/disable frustum culling
    void setFrustumCullingEnabled(bool enabled) { frustumCullingEnabled = enabled; }
    
    // Get rendering statistics
    struct RenderStats {
        int drawCalls;
        int spritesRendered;
        int spritesCulled;
        int batchesCreated;
        float lastFrameTime;
        
        RenderStats() : drawCalls(0), spritesRendered(0), spritesCulled(0), batchesCreated(0), lastFrameTime(0.0f) {}
    };
    
    const RenderStats& getStats() const { return stats; }
    void resetStats() { stats = RenderStats(); }
    
private:
    // OpenGL resources
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    
    // Batching data
    std::vector<SpriteBatch> batches;
    std::vector<SpriteVertex> vertices;
    std::vector<GLuint> indices;
    glm::mat4 viewProjectionMatrix;
    
    // Configuration
    int maxSpritesPerBatch;
    int maxTexturesPerBatch;
    
    // Frustum culling
    bool frustumCullingEnabled;
    glm::vec2 frustumMin, frustumMax, frustumSize;
    
    // Statistics
    RenderStats stats;
    
    // Shader setup
    void createShader();
    void setupBuffers();
    
    // Batching logic
    void createBatches();
    void renderBatches();
    void flushBatch(const SpriteBatch& batch);
    
    // Culling
    bool isInFrustum(const glm::vec3& position, const glm::vec2& size) const;
    
    // Vertex generation
    void generateQuadVertices(const SpriteRenderCommand& cmd, int vertexIndex, float textureIndex);
};

class SpriteRenderManager {
public:
    static SpriteRenderManager& getInstance();
    
    void init();
    void shutdown();
    
    void beginFrame(const glm::mat4& viewProjectionMatrix);
    void endFrame();
    
    // Render a sprite using atlas
    void renderSprite(const std::string& spriteName,
                      const glm::mat4& transform,
                      const glm::vec4& color = glm::vec4(1.0f),
                      int layer = 0);
    
    // Render a sprite with direct texture
    void renderSprite(GLuint textureID,
                      const glm::mat4& transform,
                      const glm::vec4& color = glm::vec4(1.0f),
                      const glm::vec2& uvMin = glm::vec2(0.0f, 0.0f),
                      const glm::vec2& uvMax = glm::vec2(1.0f, 1.0f),
                      int layer = 0);
    
    // Configuration
    void setFrustumCullingEnabled(bool enabled);
    void updateFrustum(const glm::vec2& cameraPos, const glm::vec2& viewSize, float zoom = 1.0f);
    
    // Statistics
    const SpriteBatcher::RenderStats& getStats() const;
    void resetStats();
    
private:
    std::unique_ptr<SpriteBatcher> batcher;
    bool initialized;
    
    SpriteRenderManager() : initialized(false) {}
};