#pragma once
#include <iostream>
#include "../System.hpp"
#include "../Components.hpp"
#include "../../InputManager.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
class CameraSystem3D : public System {
public:
    size_t cursorEntity;
    size_t playerEntity;
    ECS* ecs = nullptr;
    glm::vec2 mousePanDirection = {0,0};
    glm::vec2 latestMouse = {0,0};
    CameraSystem3D() {
        setSignature({typeid(CameraComponent3D).hash_code()});
        InputManager::getInstance().subscribe(InputEventType::MOUSE_MOVE, [this](const InputEvent& event) {
            float deltaX = event.mouseX - latestMouse.x;
            float deltaY = event.mouseY - latestMouse.y;
            latestMouse.x = event.mouseX;
            latestMouse.y = event.mouseY;
            mousePanDirection = glm::vec2(deltaX, deltaY);
            //mousePanDirection = event.mouseX;
            //latestMouseY = event.mouseY;
        });
    }

void update(float deltaTime, ECS& ecs) override {
    
    auto gameState = ecs.getEntitiesBySignature({typeid(GameStateComponent).hash_code()});
    auto& gameStateComponent = ecs.getComponent<GameStateComponent>(gameState[0]);
    if(gameStateComponent.isGameplayFrozen) return;
    
    auto entities = ecs.getEntitiesBySignature(signature);

    for (auto entity : entities) {

        auto& camera = ecs.getComponent<CameraComponent3D>(entity);
        auto& playerTransform = ecs.getComponent<TransformComponent>(playerEntity);
        float moveSpeed = 0.1f;


        float playerDistance = 5.0f; // Distance in front of the camera (adjust as needed)
        glm::vec3 playerOffset = camera.direction * playerDistance; 
        playerOffset.y -= 1.5f;
        glm::vec3 playerPosition = playerTransform.position - playerOffset;

        camera.yaw += mousePanDirection.x * 0.04;
        playerTransform.rotation.y -= glm::radians(mousePanDirection.x * 0.04);
        camera.pitch -= mousePanDirection.y * 0.04;
        camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);
        mousePanDirection = {0,0};
        camera.position = playerPosition;
        
        
        camera.updateVectors();
        camera.updateMatrices();
    }
}
    void onScroll(float scrollOffsetY) {
        cameraZoom *= (scrollOffsetY > 0) ? 1.1f : 0.9f;
        cameraZoom = std::clamp(cameraZoom, 0.05f, 15.0f);
    }
private:
    float cameraZoom = 1.0f;
        void printProjectionMatrix(const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
};