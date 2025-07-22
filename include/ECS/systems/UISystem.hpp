#pragma once
#include <iostream>
#include <string>
#include "../../util/Transforms.hpp"
#include "../../UIRenderer.hpp"
#include "../System.hpp"
#include "../Components.hpp"
#include "../ArchetypeManager.hpp"

class UISystem : public System {
public:
    size_t cursor;
    size_t worldTracker;
    size_t screenTracker;
    size_t camera;
    UISystem() {

        setSignature({
            typeid(UITextElement).hash_code(),
            typeid(UIImageElement).hash_code(),
            typeid(UITransform).hash_code(),
        });
    }

void update(float deltaTime, ECS& ecs) override {
    // Get entities matching the desired signature
    auto entities = ecs.getEntitiesBySignature(signature);
    
    if (ecs.hasComponent<UITextElement>(worldTracker) && 
        ecs.hasComponent<UITextElement>(screenTracker) && 
        ecs.hasComponent<CursorComponent>(cursor) && 
        ecs.hasComponent<CameraComponent2D>(camera)
        ) {

        auto& cursorComponent = ecs.getComponent<CursorComponent>(cursor);
        auto& screenText = ecs.getComponent<UITextElement>(screenTracker);
        auto& worldText = ecs.getComponent<UITextElement>(worldTracker);
        auto& cameraComponent = ecs.getComponent<CameraComponent2D>(camera);
        screenText.text = "Screen Coordinates: " + std::to_string(static_cast<int>(cursorComponent.mouseX)) + " , " + std::to_string(static_cast<int>(cursorComponent.mouseY));
        glm::vec2 worldPos = screenToWorld(cursorComponent.mouseX, cursorComponent.mouseY, cameraComponent);
        worldText.text = "World Coordinates: " + std::to_string(worldPos.x) + " , " + std::to_string(worldPos.y);

    }
    for(auto entity : entities){
        if(ecs.hasComponent<TextBoxComponent>(entity)){
            auto& textbox = ecs.getComponent<TextBoxComponent>(entity);
            auto& textElement = ecs.getComponent<UITextElement>(entity);
            textElement.text = textbox.text;
        }
    }
    
    // Render the UI entities
    uiRenderer.render(entities, &ecs);
    
}

    
private: 
    UIRenderer uiRenderer;
};