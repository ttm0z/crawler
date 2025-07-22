#ifndef WINDOW_H
#define WINDOW_H
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "InputEvent.hpp"
#include <vector>
#include <iostream>
#include "InputManager.hpp"

class Window {
public:
    static std::vector<InputEvent> eventBuffer;
    Window(int width, int height);
    ~Window();

    void update();
    
    void clear();
    bool shouldClose() const;
    
    std::vector<InputEvent> pollEvents();
    void swapBuffers();
    GLFWwindow* getWindow();
    
    void processInput();
    
    int getWidth() const;
    int getHeight() const;
    
    double getMousePosX();
    double getMousePosY();
    bool isKeyPressed(int key);
    std::vector<int> getFramebufferCoordinates();

    void destroy();

private:
    int width;
    int height;

    double mousePosX;
    double mousePosY;

    const char* title;
    GLFWwindow* window;

void handleKeyEvent(int key, int action, int mods) {
    // If it's a repeat event, we don't want to add a new KEY_PRESS event to the buffer.
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        InputEventType type = (action == GLFW_PRESS) ? InputEventType::KEY_PRESS : InputEventType::KEY_PRESS_REPEAT;
        eventBuffer.push_back({type, key});
    } else if (action == GLFW_RELEASE) {
        eventBuffer.push_back({InputEventType::KEY_RELEASE, key});
    }
}



    void handleMouseMove(double xpos, double ypos) {
        
        eventBuffer.push_back({InputEventType::MOUSE_MOVE, 0, static_cast<int>(xpos), static_cast<int>(ypos)});
        

    }

    void handleScroll(int xOffset, int yOffset) {
        
        eventBuffer.push_back({InputEventType::MOUSE_SCROLL, 0, static_cast<int>(xOffset), static_cast<int>(yOffset), static_cast<int>(yOffset)});
    }

void handleMouseButton(int button, int action) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    InputEventType type;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        type = (action == GLFW_PRESS) ? InputEventType::MOUSE_BUTTON_LEFT : InputEventType::MOUSE_BUTTON_LEFT_RELEASE;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        type = (action == GLFW_PRESS) ? InputEventType::MOUSE_BUTTON_RIGHT : InputEventType::MOUSE_BUTTON_RIGHT_RELEASE;
    } else {
        return; // Ignore other buttons
    }

    // Dispatch event to InputManager
    InputManager::getInstance().handleMouseEvent(button, action);

    // Push the correct event type into the event buffer
    eventBuffer.push_back({type, button, xpos, ypos});
}


    
};
#endif // WINDOW_H
