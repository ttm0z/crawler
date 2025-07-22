#pragma once
#include "../System.hpp"
#include "../Components.hpp"
#include <iostream>
#include "../../InputManager.hpp"

class UIInputSystem : public System {
public:
    UIInputSystem() {
        setSignature({
            typeid(UITransform).hash_code(),
            typeid(UIInput).hash_code()
        });

        // Subscribe to mouse move events
        InputManager::getInstance().subscribe(InputEventType::MOUSE_MOVE, [this](const InputEvent& event) {
            latestMouseX = event.mouseX;
            latestMouseY = event.mouseY;
        });
    }

    void update(float deltaTime, ECS& ecs) override {
        // Get all entities matching the signature
        auto entities = ecs.getEntitiesBySignature(signature);
        for (auto entity : entities) {
        auto& transformComponent = ecs.getComponent<UITransform>(entity);
        auto& inputComponent = ecs.getComponent<UIInput>(entity);
        bool isHovered = (
            latestMouseX >= (transformComponent.position[0]) - transformComponent.size[0] / 2.0f &&
            latestMouseX <= (transformComponent.position[0] + transformComponent.size[0] / 2.0f) &&
            latestMouseY >= (800 - transformComponent.position[1]) - transformComponent.size[1] / 2.0f &&
            latestMouseY <= (800 - transformComponent.position[1] + transformComponent.size[1] / 2.0f));
        bool isClicked = isHovered && InputManager::getInstance().isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        if (ecs.hasComponent<UITextElement>(entity) && 
            ! ecs.hasComponent<PlayerSlot>(entity)
        ){
            auto& text = ecs.getComponent<UITextElement>(entity);
            auto& transform = ecs.getComponent<UITransform>(entity);
            if(isHovered){
                text.color = glm::vec3(1.0f, 0.1f, 0.2f);
                transform.scale = glm::vec2(1.05f);
            }
            else {
                text.color = glm::vec3(0.0f);
                transform.scale = glm::vec2(1.0f);
            }
            
        }
        inputComponent.handleInput(isClicked, isHovered);        
        }
    }



private:
    float latestMouseX = 0.0f;   // Latest mouse X position
    float latestMouseY = 0.0f;   // Latest mouse Y position
};


