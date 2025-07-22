#pragma once
#include <vector>
#include <memory>
#include "GameState.hpp"
#include "EventQueue.hpp"
class GameStateManager {
public:
    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;

    static GameStateManager& getInstance() {
        static GameStateManager instance;
        return instance;
    }
    void pushState(std::unique_ptr<GameState> state);

    void popState();
    
    GameState* getCurrentState() const;
    
    EventQueue& getEventQueue();

    void processEvents();

    void replaceState(std::unique_ptr<GameState> state);

    void update(float deltaTime);

    bool isEmpty() const;

private:
    GameStateManager() {}
    std::vector<std::unique_ptr<GameState>> states;
    EventQueue eventQueue;
};
