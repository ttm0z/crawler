#pragma once
#include <vector>
#include "ECS.hpp"
#include "../GameplayEventQueue.hpp"
class ECS;

class System {

protected:
    std::vector<size_t> signature;

public:
    GameplayEventQueue* eventQueue = nullptr;
    virtual void update(float deltaTime, ECS& ecs) = 0;

    void setSignature(const std::vector<size_t>& componentTypes) {
        signature = componentTypes;
    }

    const std::vector<size_t>& getSignature() const {
        return signature;
    }

    void setEventQueue(GameplayEventQueue* queue) {
        eventQueue = queue;
    }
};