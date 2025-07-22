#pragma once
#include <memory>
#include "ECS/Components.hpp"
#include "ECS/ECS.hpp"
#include "EventQueue.hpp"
#include "InputManager.hpp"

// Gamestate base class definition.
class GameState {
public:
    
    GameState(EventQueue& eventQueue) 
        : eventQueue(eventQueue), ecs(std::make_unique<ECS>()) {}  

    virtual ~GameState() {}

    virtual void update(float deltaTime) = 0;
    virtual void onEnter() {}  // Optional: called when state is activated
    virtual void onExit() {}   // Optional: called when state is deactivated

protected:
    
    std::unique_ptr<ECS> ecs;
    EventQueue& eventQueue;
};
