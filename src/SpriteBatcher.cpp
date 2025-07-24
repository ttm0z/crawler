#include "SpriteBatcher.hpp"
#include "TextureAtlas.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>

// Shader source code for sprite batching
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTextureIndex;

uniform mat4 uViewProjection;

out vec2 vTexCoord;
out vec4 vColor;
flat out int vTextureIndex;

void main() {
    gl_Position = uViewProjection * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
    vColor = aColor;
    vTextureIndex = int(aTextureIndex);
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec2 vTexCoord;
in vec4 vColor;
flat in int vTextureIndex;

uniform sampler2D uTextures[8];

out vec4 FragColor;

void main() {
    vec4 texColor = vec4(1.0);
    
    // Dynamic texture indexing
    if (vTextureIndex == 0) texColor = texture(uTextures[0], vTexCoord);
    else if (vTextureIndex == 1) texColor = texture(uTextures[1], vTexCoord);
    else if (vTextureIndex == 2) texColor = texture(uTextures[2], vTexCoord);
    else if (vTextureIndex == 3) texColor = texture(uTextures[3], vTexCoord);
    else if (vTextureIndex == 4) texColor = texture(uTextures[4], vTexCoord);
    else if (vTextureIndex == 5) texColor = texture(uTextures[5], vTexCoord);
    else if (vTextureIndex == 6) texColor = texture(uTextures[6], vTexCoord);
    else if (vTextureIndex == 7) texColor = texture(uTextures[7], vTexCoord);
    
    FragColor = texColor * vColor;
    
    // Discard fully transparent pixels
    if (FragColor.a < 0.01) {
        discard;
    }
}
)glsl";

SpriteBatcher::SpriteBatcher(int maxSprites, int maxTextures) 
    : VAO(0), VBO(0), EBO(0), shaderProgram(0)
    , maxSpritesPerBatch(maxSprites), maxTexturesPerBatch(maxTextures)
    , frustumCullingEnabled(false)
    , frustumMin(-1000.0f), frustumMax(1000.0f), frustumSize(2000.0f) {
    
    // Pre-allocate vectors for performance
    vertices.reserve(maxSprites * 4); // 4 vertices per sprite
    indices.reserve(maxSprites * 6);  // 6 indices per sprite (2 triangles)
    batches.reserve(10); // Reasonable number of batches
}

SpriteBatcher::~SpriteBatcher() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}

void SpriteBatcher::init() {
    createShader();
    setupBuffers();
    std::cout << "SpriteBatcher initialized successfully" << std::endl;
}

void SpriteBatcher::createShader() {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }
    
    // Link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }
    
    // Clean up individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Set up texture uniforms
    glUseProgram(shaderProgram);
    for (int i = 0; i < maxTexturesPerBatch; ++i) {
        std::string uniformName = "uTextures[" + std::to_string(i) + "]";
        glUniform1i(glGetUniformLocation(shaderProgram, uniformName.c_str()), i);
    }
}

void SpriteBatcher::setupBuffers() {
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    // Set up VBO (dynamic buffer for sprite vertices)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maxSpritesPerBatch * 4 * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, position));
    glEnableVertexAttribArray(0);
    
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texCoord));
    glEnableVertexAttribArray(1);
    
    // Color attribute
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, color));
    glEnableVertexAttribArray(2);
    
    // Texture index attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, textureIndex));
    glEnableVertexAttribArray(3);
    
    // Set up EBO (index buffer for quad indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxSpritesPerBatch * 6 * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    
    glBindVertexArray(0);
}

void SpriteBatcher::begin(const glm::mat4& viewProjection) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    viewProjectionMatrix = viewProjection;
    
    // Clear previous frame data
    for (auto& batch : batches) {
        batch.clear();
    }
    batches.clear();
    vertices.clear();
    indices.clear();
    
    // Reset stats
    stats.drawCalls = 0;
    stats.spritesRendered = 0;
    stats.spritesCulled = 0;
    stats.batchesCreated = 0;
    
    auto endTime = std::chrono::high_resolution_clock::now();
    stats.lastFrameTime = std::chrono::duration<float, std::milli>(endTime - startTime).count();
}

void SpriteBatcher::addSprite(const glm::mat4& transform, const glm::vec4& color, GLuint textureID, 
                              const glm::vec2& uvMin, const glm::vec2& uvMax, int layer) {
    
    // Extract position and size from transform for culling
    glm::vec3 position = glm::vec3(transform[3]);
    glm::vec3 scale = glm::vec3(glm::length(transform[0]), glm::length(transform[1]), glm::length(transform[2]));
    
    // Frustum culling check
    if (frustumCullingEnabled && !isInFrustum(position, glm::vec2(scale))) {
        stats.spritesCulled++;
        return;
    }
    
    // Create render command
    SpriteRenderCommand cmd;
    cmd.transform = transform;
    cmd.color = color;
    cmd.uvMin = uvMin;
    cmd.uvMax = uvMax;
    cmd.layer = layer;
    
    // Find or create appropriate batch
    SpriteBatch* targetBatch = nullptr;
    for (auto& batch : batches) {
        if (batch.canAddTexture(textureID)) {
            targetBatch = &batch;
            break;
        }
    }
    
    // Create new batch if needed
    if (!targetBatch) {
        batches.emplace_back(maxTexturesPerBatch);
        targetBatch = &batches.back();
        stats.batchesCreated++;
    }
    
    // Add texture to batch and get index
    cmd.textureIndex = targetBatch->getTextureIndex(textureID);
    
    // Add command to batch
    targetBatch->commands.push_back(cmd);
    stats.spritesRendered++;
}

void SpriteBatcher::end() {
    if (batches.empty()) return;
    
    // Sort batches by layer for proper depth ordering
    for (auto& batch : batches) {
        std::sort(batch.commands.begin(), batch.commands.end(),
            [](const SpriteRenderCommand& a, const SpriteRenderCommand& b) {
                return a.layer < b.layer;
            });
    }
    
    createBatches();
    renderBatches();
}

void SpriteBatcher::createBatches() {
    vertices.clear();
    indices.clear();
    
    int vertexIndex = 0;
    
    for (const auto& batch : batches) {
        for (const auto& cmd : batch.commands) {
            // Generate quad vertices for this sprite
            generateQuadVertices(cmd, vertexIndex, cmd.textureIndex);
            
            // Generate indices for this quad (2 triangles)
            int baseIndex = vertexIndex;
            indices.insert(indices.end(), {
                baseIndex + 0, baseIndex + 1, baseIndex + 2,  // First triangle
                baseIndex + 2, baseIndex + 3, baseIndex + 0   // Second triangle
            });
            
            vertexIndex += 4; // 4 vertices per quad
        }
    }
}

void SpriteBatcher::generateQuadVertices(const SpriteRenderCommand& cmd, int vertexIndex, float textureIndex) {
    // Define unit quad vertices (centered at origin)
    glm::vec3 quadVertices[4] = {
        glm::vec3(-0.5f, -0.5f, 0.0f), // Bottom-left
        glm::vec3( 0.5f, -0.5f, 0.0f), // Bottom-right  
        glm::vec3( 0.5f,  0.5f, 0.0f), // Top-right
        glm::vec3(-0.5f,  0.5f, 0.0f)  // Top-left
    };
    
    // UV coordinates for the quad
    glm::vec2 uvCoords[4] = {
        glm::vec2(cmd.uvMin.x, cmd.uvMin.y), // Bottom-left
        glm::vec2(cmd.uvMax.x, cmd.uvMin.y), // Bottom-right
        glm::vec2(cmd.uvMax.x, cmd.uvMax.y), // Top-right
        glm::vec2(cmd.uvMin.x, cmd.uvMax.y)  // Top-left
    };
    
    // Transform vertices and create sprite vertices
    for (int i = 0; i < 4; ++i) {
        glm::vec3 worldPos = glm::vec3(cmd.transform * glm::vec4(quadVertices[i], 1.0f));
        
        vertices.emplace_back(
            worldPos,
            uvCoords[i],
            cmd.color,
            textureIndex
        );
    }
}

void SpriteBatcher::renderBatches() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    
    // Upload view-projection matrix
    GLint vpLocation = glGetUniformLocation(shaderProgram, "uViewProjection");
    glUniformMatrix4fv(vpLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(SpriteVertex), vertices.data());
    
    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
    
    // Render each batch
    int indexOffset = 0;
    for (const auto& batch : batches) {
        flushBatch(batch);
        
        // Draw this batch
        int indexCount = batch.commands.size() * 6; // 6 indices per sprite
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(indexOffset * sizeof(GLuint)));
        
        indexOffset += indexCount;
        stats.drawCalls++;
    }
    
    glBindVertexArray(0);
}

void SpriteBatcher::flushBatch(const SpriteBatch& batch) {
    // Bind all textures used in this batch
    for (size_t i = 0; i < batch.textureIDs.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, batch.textureIDs[i]);
    }
}

void SpriteBatcher::setFrustumBounds(const glm::vec2& min, const glm::vec2& max, const glm::vec2& size) {
    frustumMin = min;
    frustumMax = max;
    frustumSize = size;
}

bool SpriteBatcher::isInFrustum(const glm::vec3& position, const glm::vec2& size) const {
    // Simple AABB frustum culling
    glm::vec2 pos2D = glm::vec2(position);
    glm::vec2 halfSize = size * 0.5f;
    
    return !(pos2D.x + halfSize.x < frustumMin.x || 
             pos2D.x - halfSize.x > frustumMax.x ||
             pos2D.y + halfSize.y < frustumMin.y || 
             pos2D.y - halfSize.y > frustumMax.y);
}

// SpriteRenderManager implementation
SpriteRenderManager& SpriteRenderManager::getInstance() {
    static SpriteRenderManager instance;
    return instance;
}

void SpriteRenderManager::init() {
    if (initialized) return;
    
    batcher = std::make_unique<SpriteBatcher>(1000, 8);
    batcher->init();
    initialized = true;
    
    std::cout << "SpriteRenderManager initialized" << std::endl;
}

void SpriteRenderManager::shutdown() {
    batcher.reset();
    initialized = false;
}

void SpriteRenderManager::beginFrame(const glm::mat4& viewProjectionMatrix) {
    if (!initialized) return;
    batcher->begin(viewProjectionMatrix);
}

void SpriteRenderManager::endFrame() {
    if (!initialized) return;
    batcher->end();
}

void SpriteRenderManager::renderSprite(const std::string& spriteName, const glm::mat4& transform, 
                                       const glm::vec4& color, int layer) {
    if (!initialized) return;
    
    // Find atlas containing the sprite
    auto atlas = TextureAtlasManager::getInstance().findAtlasForSprite(spriteName);
    if (!atlas) {
        std::cerr << "Sprite not found in any atlas: " << spriteName << std::endl;
        return;
    }
    
    // Get sprite UV coordinates
    const SpriteUV* spriteUV = atlas->getSpriteUV(spriteName);
    if (!spriteUV) {
        std::cerr << "Sprite UV data not found: " << spriteName << std::endl;
        return;
    }
    
    // Render using atlas texture
    batcher->addSprite(transform, color, atlas->getTextureID(), 
                       spriteUV->uv0, spriteUV->uv1, layer);
}

void SpriteRenderManager::renderSprite(GLuint textureID, const glm::mat4& transform, 
                                       const glm::vec4& color, const glm::vec2& uvMin, 
                                       const glm::vec2& uvMax, int layer) {
    if (!initialized) return;
    batcher->addSprite(transform, color, textureID, uvMin, uvMax, layer);
}

void SpriteRenderManager::setFrustumCullingEnabled(bool enabled) {
    if (!initialized) return;
    batcher->setFrustumCullingEnabled(enabled);
}

void SpriteRenderManager::updateFrustum(const glm::vec2& cameraPos, const glm::vec2& viewSize, float zoom) {
    if (!initialized) return;
    
    glm::vec2 halfSize = viewSize * 0.5f / zoom;
    glm::vec2 min = cameraPos - halfSize;
    glm::vec2 max = cameraPos + halfSize;
    
    batcher->setFrustumBounds(min, max, viewSize);
}

const SpriteBatcher::RenderStats& SpriteRenderManager::getStats() const {
    static SpriteBatcher::RenderStats emptyStats;
    return initialized ? batcher->getStats() : emptyStats;
}

void SpriteRenderManager::resetStats() {
    if (initialized) batcher->resetStats();
}