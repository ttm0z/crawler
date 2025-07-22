#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include "InputEvent.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "util/Transforms.hpp"

class InputManager {
public:

    static InputManager& getInstance();

    // Prevent copying and assignment
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    
    void processInput(InputEvent event);
    bool isMouseButtonPressed(int button) const;
    void subscribe(InputEventType eventType, std::function<void(const InputEvent&)> observer);
    void updateKeyState(int keycode, bool isPressed);
    bool isKeyPressed(int keyCode) const;
    void reset();
    void handleMouseEvent(int button, int action);
    void processMouseMovement(double xpos, double ypos) {
        InputEvent event(InputEventType::MOUSE_MOVE, xpos, ypos);
        processInput(event);
    }

private:
    InputManager();
    std::unordered_map<InputEventType, std::vector<std::function<void(const InputEvent&)>>> observers;
    std::unordered_map<int, bool> mouseButtonStates;
    std::unordered_map<int, bool> keyStates;
};
