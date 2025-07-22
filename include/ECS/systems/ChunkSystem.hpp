#pragma once
#include <vector>
#include <cmath>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "../System.hpp"
#include "util/Render.hpp"
#include <algorithm>
#include "../Components.hpp"
#include "../../ResourceManager.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For transformations
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr
struct ivec3_hash {
    std::size_t operator()(const glm::ivec3 &v) const {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        std::size_t h3 = std::hash<int>()(v.z);
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

class ChunkSystem : public System {
public:
    int viewDistance = 3;
    int chunkSize = 16;
    size_t cameraEntity;
    size_t w_coord;
    size_t t_coord;
    size_t c_coord;
    size_t player;
    std::vector<std::vector<ChunkData>> map;
    std::vector<size_t> activeChunks;
    std::vector<size_t> inactiveChunks;
    std::unordered_map<glm::ivec3, size_t, ivec3_hash> chunkRegistry;
    glm::ivec2 cameraChunk = glm::ivec2(-2);
    
    
    ChunkSystem(){
        setSignature({
            typeid(ChunkComponent).hash_code()
        });
    }


    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        auto& camera = ecs.getComponent<CameraComponent3D>(cameraEntity);
        auto& playerTransform = ecs.getComponent<TransformComponent>(player);
        for (auto entity : activeChunks){
            auto& chunk = ecs.getComponent<ChunkComponent>(entity);
            auto& renderable = ecs.getComponent<RenderableComponent>(entity);
            chunk.toDraw = false;
            if(chunk.dirty){
                std::vector<GLfloat> vertices;
                std::vector<GLint> indices;
                updateChunkGeometry(chunk, renderable, vertices, indices);
                chunk.dirty = false;
            }
        }
        
        
        glm::ivec2 camChunkPos = worldToChunkCoord(playerTransform.position, chunkSize, 1.0f);
        glm::ivec3 tilePosition = worldToLocalTileCoord(playerTransform.position, camChunkPos, chunkSize, 1.0f);
        if (camChunkPos != cameraChunk) {
            activeChunks.clear();
            updateActiveChunks(ecs, playerTransform.position);
        }
            
        cameraChunk = camChunkPos;

        auto& worldLabel = ecs.getComponent<UITextElement>(w_coord);
        worldLabel.text = "World Coordinates: " + std::to_string(playerTransform.position.x) + ", " + 
        std::to_string(playerTransform.position.y) + ", " + std::to_string(playerTransform.position.z);
        int gridX = static_cast<int>(floor((camChunkPos.x + map[0].size() / 2)));
        int gridY = static_cast<int>(floor((camChunkPos.y + map.size() / 2)));
        auto& chunkLabel = ecs.getComponent<UITextElement>(c_coord);

        if ((gridX >= 0 && gridX < map[0].size()) && (gridY >= 0 && gridY < map.size())) {
            chunkLabel.text = "Chunk Coordinates: " + std::to_string(camChunkPos.x) + ", " + std::to_string(camChunkPos.y) + " elevation" + std::to_string(map[gridX][gridY].finalElevation);
        }

        auto& tileLabel = ecs.getComponent<UITextElement>(t_coord);
        tileLabel.text = "Tile Coordinates: " + std::to_string(tilePosition.x) + ", " + 
            std::to_string(tilePosition.y) + ", " + std::to_string(tilePosition.z);
        

    
    }

    void updateActiveChunks(ECS& ecs, const glm::vec3& camWorldPos) {
        // Compute the camera's chunk coordinate.
        glm::ivec2 chCoord = worldToChunkCoord(camWorldPos, chunkSize, 1.0f);
        glm::ivec3 camChunkPos = {chCoord.x, camWorldPos.y, chCoord.y};
        
        int gridX = static_cast<int>(floor((camChunkPos.x + map[0].size() / 2)));
        int gridY = static_cast<int>(floor((camChunkPos.z + map.size() / 2)));

        
        for (int x = -viewDistance; x <= viewDistance; ++x) {
            for (int y = -viewDistance; y <= viewDistance; ++y) {
                int posx = gridX + x;
                int posy = gridY + y;
                glm::ivec3 chunkCoord = glm::ivec3(x + camChunkPos.x, y + camChunkPos.z, 0);
                if (chunkExists(chunkCoord)) {
                    auto chunk = getChunkEntity(chunkCoord);
                    auto& chunkComponent = ecs.getComponent<ChunkComponent>(chunk);
                    activeChunks.push_back(getChunkEntity(chunkCoord));
                    chunkComponent.toDraw = true;
                } else {
                    int elevation = 0;
                    if ((posx >= 0 && posx < map[0].size()) && (posy >= 0 && posy < map.size())) 
                    {
                        elevation = map[posx][posy].finalElevation;
                    }
                    ChunkComponent chunk(chunkCoord, chunkSize, chunkSize, 40, 1.0f, elevation);
                    size_t chunkEntity = addChunk(&ecs, map, chunk);
                    chunkRegistry.insert({chunkCoord, chunkEntity});
                }
            }
        }
    }
    bool chunkExists(const glm::ivec3 &chunkCoord) {
        return (chunkRegistry.find(chunkCoord) != chunkRegistry.end());
    }
    size_t getChunkEntity(const glm::ivec3 &chunkCoord) {
        return chunkRegistry[chunkCoord];
    }
};