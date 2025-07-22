#pragma once
#include <iostream>
#include "../System.hpp"
#include "../Components.hpp"
#include "../../InputManager.hpp"
#include "ChunkSystem.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
class PhysicsSystem : public System {
public:
    ChunkSystem* chunkSystem;
    glm::vec3 gravity = {0.0f, -2.08f, 0.0f};
    
    PhysicsSystem() {
        setSignature({typeid(PhysicsComponent2D).hash_code(), typeid(TransformComponent2D).hash_code()});  
    }

void update(float deltaTime, ECS& ecs) override {
        // Temporarily disabled for 2D conversion
        /*
    auto entities = ecs.getEntitiesBySignature(signature);
    
    for (auto entity : entities) {
        auto& physics = ecs.getComponent<PhysicsComponent2D>(entity);
        auto& transform = ecs.getComponent<TransformComponent2D>(entity);

        // Apply acceleration to velocity
        physics.velocity *= 0.99f;
        physics.velocity += physics.acceleration * deltaTime;

        // Predict next position
        glm::vec3 newPosition = transform.position + physics.velocity * deltaTime;

        // Convert world position to chunk/tile coordinates
        glm::ivec2 chunkCoord = worldToChunkCoord(newPosition, 16, 1.0f);
        glm::ivec3 tileCoord = worldToLocalTileCoord(newPosition, chunkCoord, 16, 1.0f);
        size_t currentChunk = chunkSystem -> getChunkEntity({chunkCoord.x, chunkCoord.y, 0});

        if (ecs.hasComponent<ChunkComponent>(currentChunk)) {
            auto& chunk = ecs.getComponent<ChunkComponent>(currentChunk);

            if (ecs.hasComponent<ColliderComponent>(entity)) {
                auto& collider = ecs.getComponent<ColliderComponent>(entity);
                
                // Check collisions separately for each axis
                glm::vec3 tempPosition = transform.position;
                glm::vec3 tempVelocity = physics.velocity;

                // Test movement in X direction
                tempPosition.x = transform.position.x + physics.velocity.x * deltaTime;
                if (checkCollision(tempPosition, collider, chunk)) {
                    physics.velocity.x = 0.0f;
                }

                // Test movement in Y direction
                tempPosition = transform.position; // Reset position
                tempPosition.y = transform.position.y + physics.velocity.y * deltaTime;
                if (checkCollision(tempPosition, collider, chunk)) {
                    physics.velocity.y = 0.0f;
                    physics.onGround = true;
                } else {
                    physics.onGround = false;
                }

                // Test movement in Z direction
                tempPosition = transform.position;
                tempPosition.z = transform.position.z + physics.velocity.z * deltaTime;
                if (checkCollision(tempPosition, collider, chunk)) {
                    physics.velocity.z = 0.0f;
                }

                // Apply valid movement
                transform.position += physics.velocity * deltaTime;
            }
        }

        // Apply gravity if not grounded
        if (!physics.onGround) {
            physics.acceleration = gravity;
        } else {
            physics.acceleration = glm::vec3(0.0f);
        }

        // Reset acceleration at the end of frame
        
    }
        */
    }

/*
bool checkCollision(const glm::vec3& position, const ColliderComponent2D& collider, DungeonRoomComponent& room) {

    glm::vec3 minCorner = position - collider.size * 0.5f;
    glm::vec3 maxCorner = position + collider.size * 0.5f;

    float hexDiameter = chunk.hexRadius; // Actually diameter
    float hexRadius = hexDiameter / 2.0f;
    float hexHeight = hexRadius * sqrt(3.0f);
    float tileHeight = 0.5f; // vertical height of tiles

    // Calculate vertical (layer) bounds
    int minLayer = floor(minCorner.y / tileHeight);
    int maxLayer = floor(maxCorner.y / tileHeight);

    // Calculate horizontal hex bounds using correct offsets
    int minQ = floor(minCorner.x / (hexRadius * 1.5f));
    int maxQ = ceil(maxCorner.x / (hexRadius * 1.5f));

    int minR = floor(minCorner.z / hexHeight);
    int maxR = ceil(maxCorner.z / hexHeight);

    glm::ivec2 chunkCoord = worldToChunkCoord(position, 16, 1.0f);

    for (int layer = minLayer; layer <= maxLayer; ++layer) {
        float tileBottom = layer * tileHeight;
        float tileTop = (layer + 1) * tileHeight;

        for (int q = minQ; q <= maxQ; ++q) {
            for (int r = minR; r <= maxR; ++r) {

                glm::vec3 center = getTilePosition(q, r, hexDiameter);
                center.y = tileBottom + tileHeight / 2.0f;

                // Precise AABB bounds for hex tile
                glm::vec3 tileMin(center.x - hexRadius * 0.75f, tileBottom, center.z - hexHeight / 2.0f);
                glm::vec3 tileMax(center.x + hexRadius * 0.75f, tileTop, center.z + hexHeight / 2.0f);

                // Early rejection
                if (maxCorner.x < tileMin.x || minCorner.x > tileMax.x ||
                    maxCorner.y < tileMin.y || minCorner.y > tileMax.y ||
                    maxCorner.z < tileMin.z || minCorner.z > tileMax.z)
                    continue;

                // Get tile coordinate correctly
                glm::ivec3 tileCoord = worldToLocalTileCoord(center, chunkCoord, 16, 1.0f);
                int tileIndex = chunk.indexFrom3D(tileCoord.x, tileCoord.y, tileCoord.z);

                if (chunk.occupancy[tileIndex] == 1) {
                    
                    return true;
                }
            }
        }
    }

    return false; // No collision detected
}
*/


private:

};