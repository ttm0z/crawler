#pragma once
#include <memory>

class GameState;
enum class GameEventType {
    ChangeState,
    PushState,
    PopState,
    ExitApplication
};

struct GameEvent {
    GameEventType type;
    std::unique_ptr<GameState> newState = nullptr; // Optional for state change

    // Constructor for GameEvent
    GameEvent(GameEventType type, std::unique_ptr<GameState> newState = nullptr)
        : type(type), newState(std::move(newState)) {}

    // Delete copy constructor and copy assignment operator
    GameEvent(const GameEvent&) = delete;
    GameEvent& operator=(const GameEvent&) = delete;

    // Define move constructor and move assignment operator
    GameEvent(GameEvent&& other) noexcept = default;
    GameEvent& operator=(GameEvent&& other) noexcept = default;
};

