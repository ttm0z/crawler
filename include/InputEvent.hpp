
#pragma once
enum class InputEventType {
    KEY_PRESS,
    KEY_PRESS_REPEAT,
    KEY_RELEASE,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_LEFT_RELEASE,
    MOUSE_BUTTON_RIGHT_RELEASE
};

struct InputEvent {
    InputEventType type;
    int key = 0;
    int scancode = 0;
    int mouseX = 0;
    int mouseY = 0;
    float scrollOffsetY = 0.0f;

    // Constructor
    InputEvent(InputEventType eventType, int k, int x = 0, int y = 0, float scroll = 0.0f)
        : type(eventType), key(k), mouseX(x), mouseY(y), scrollOffsetY(scroll) {}
};
