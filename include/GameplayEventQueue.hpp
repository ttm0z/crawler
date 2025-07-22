#pragma once
#include <string>
#include <memory>
#include <queue>
#include "GameplayEvent.hpp"
#include <stdexcept>

class GameplayEventQueue {
public:
    void push(std::unique_ptr<GameplayEvent> event) {
        events.push(std::move(event));
    }

    bool hasEvents() const {
        return !events.empty();
    }

    std::unique_ptr<GameplayEvent> pop() {
        if (events.empty())
            throw std::runtime_error("Empty GameplayEventQueue");
        auto e = std::move(events.front());
        events.pop();
        return e;
    }

private:
    std::queue<std::unique_ptr<GameplayEvent>> events;
};
