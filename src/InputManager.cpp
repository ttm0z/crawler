#include "InputManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For transformations
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr
#include <iostream>

InputManager& InputManager::getInstance() {
    static InputManager instance; 
    return instance;
}
InputManager::InputManager() = default;

void InputManager::handleMouseEvent(int button, int action) {
    if (action == GLFW_PRESS) {
        mouseButtonStates[button] = true;
    } else if (action == GLFW_RELEASE) {
        mouseButtonStates[button] = false;
    }

    // Log mouse button state changes
    // std::cout << "[InputManager] Mouse button " << button 
    //           << " is " << (action == GLFW_PRESS ? "pressed" : "released") << std::endl;
}

void InputManager::subscribe(InputEventType eventType, std::function<void(const InputEvent&)> observer) {
    // std::cout << "[InputManager] Subscribing to event type: " 
    //           << static_cast<int>(eventType) << std::endl;
    observers[eventType].push_back(observer);
}

void InputManager::updateKeyState(int keycode, bool isPressed) {
    if (keyStates[keycode] != isPressed) { // Only update when the state changes
        keyStates[keycode] = isPressed;
    }
}


bool InputManager::isKeyPressed(int keyCode) const {
    auto it = keyStates.find(keyCode);
    if (it != keyStates.end()) {
        return it->second;  // Returns true if key is pressed
    }
    return false;  // Default to false if not found
}


void InputManager::reset() {
    keyStates.clear();
}

bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonStates.find(button);
    return it != mouseButtonStates.end() && it->second;
}

void InputManager::processInput(InputEvent event) {
    // Log the incoming event
    if (event.type == InputEventType::MOUSE_MOVE) {
        
    } else if (event.type == InputEventType::KEY_PRESS || event.type == InputEventType::KEY_RELEASE) {
        
    }

    if (event.type == InputEventType::MOUSE_SCROLL) {
        
    }
    // Handle key state changes for key press and release
    if (event.type == InputEventType::KEY_PRESS) {
        keyStates[event.key] = true;
    } else if (event.type == InputEventType::KEY_RELEASE) {
        keyStates[event.key] = false;
    }

    // Notify observers
    if (observers.find(event.type) != observers.end()) {
        for (auto& observer : observers[event.type]) {
            observer(event);
        }
    }
}

