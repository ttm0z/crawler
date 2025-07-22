#pragma once
#include <iostream>
#include "../System.hpp"
#include "../Components.hpp"
#include "../../InputManager.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
class CameraSystem2D : public System {
public:
    size_t cursorEntity;
    CameraSystem2D() {
        setSignature({typeid(CameraComponent2D).hash_code()});
        InputManager::getInstance().subscribe(InputEventType::MOUSE_SCROLL, [this](const InputEvent& event) {
            onScroll(event.scrollOffsetY);
        });
    }
void update(float deltaTime, ECS& ecs) override {
    auto entities = ecs.getEntitiesBySignature(signature);

    for (auto entity : entities) {
        
        auto& camera = ecs.getComponent<CameraComponent2D>(entity);
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