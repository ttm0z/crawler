#include "../System.hpp"
#include "../Components.hpp"
#include <iostream>
#include "../../InputManager.hpp"

class CursorSystem : public System {
public:
    CursorSystem() {
        setSignature({
            typeid(UIImageElement).hash_code(),
            typeid(UITransform).hash_code(),
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
            if (ecs.hasComponent<CursorComponent>(entity)){
                auto& cursorComponent = ecs.getComponent<CursorComponent>(entity);
                auto& transformComponent = ecs.getComponent<UITransform>(entity);
                
                if(!cursorComponent.is3D)
                {                
                    float xOffset = (latestMouseX - cursorComponent.mouseX) * cursorComponent.sensitivity;
                    float yOffset = (cursorComponent.mouseY - latestMouseY) * cursorComponent.sensitivity;

                    // Update the cursor component with the new mouse position
                    cursorComponent.xOffset = xOffset;
                    cursorComponent.yOffset = yOffset;
                    cursorComponent.mouseX = latestMouseX;
                    cursorComponent.mouseY = latestMouseY;

                    // Synchronize transform position with screen coordinates
                    transformComponent.position.x = cursorComponent.mouseX;
                    transformComponent.position.y = 800.0f - cursorComponent.mouseY;

                    if(transformComponent.position.x < 0 || cursorComponent.mouseX < 0) {
                        transformComponent.position.x = 0;
                        cursorComponent.mouseX = 0;
                    }
                    if(transformComponent.position.x > 1200 || cursorComponent.mouseX > 1200) {
                        transformComponent.position.x = 1200;
                        cursorComponent.mouseX = 1200;
                    } 
                    if(transformComponent.position.y < 0 || cursorComponent.mouseY > 800) {
                        transformComponent.position.y = 0;
                        cursorComponent.mouseY = 800;
                    } 
                    if(transformComponent.position.y > 800 || cursorComponent.mouseY < 0) {
                        transformComponent.position.y = 800;
                        cursorComponent.mouseY = 0;
                    }
                }
                else {
                    transformComponent.position.x = 600.0f;
                    transformComponent.position.y = 400.0f;
                    cursorComponent.mouseX = 600.0f;
                    cursorComponent.mouseY = 400.0f;
                }
            }            

        }
    }

private:
    float latestMouseX = 0.0f;   // Latest mouse X position
    float latestMouseY = 0.0f;   // Latest mouse Y position
};
