#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For transformations
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr
#include "ECS/Components.hpp"
    

inline glm::vec2 screenToWorld(float screenX, float screenY, const CameraComponent2D& camera) {
    // Convert screen coordinates to Normalized Device Coordinates (NDC)
    float ndcX = (2.0f * screenX) / 1200.0f - 1.0f;
    float ndcY = 1.0f - (2.0f * screenY) / 800.0f;  // Flip Y because screen Y is inverted

    // Create a clip space coordinate (homogeneous clip coordinates)
    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

    // Convert from clip space to world space
    glm::mat4 invVP = glm::inverse(camera.projectionMatrix * camera.viewMatrix);
    glm::vec4 worldCoords = invVP * clipCoords;

    // Convert from homogeneous to 2D world coordinates
    return glm::vec2(worldCoords.x, worldCoords.y);
}


