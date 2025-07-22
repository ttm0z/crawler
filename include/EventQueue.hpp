#pragma once
#include <queue>
#include <stdexcept>
#include <memory>
#include "GameEvent.hpp"

class EventQueue {
public:
    // Default constructor
    EventQueue() = default;

    // Push a unique_ptr<GameEvent> into the queue
    void push(std::unique_ptr<GameEvent> event) {
        events.push(std::move(event));
    }

    // Push by forwarding arguments to create a GameEvent directly in the queue
    template <typename... Args>
    void emplace(Args&&... args) {
        events.emplace(std::make_unique<GameEvent>(std::forward<Args>(args)...));
    }

    bool hasEvents() const {
        return !events.empty();
    }

    // Pop and return a unique_ptr<GameEvent> from the queue
    std::unique_ptr<GameEvent> pop() {
        if (events.empty()) {
            throw std::runtime_error("EventQueue is empty!");
        }

        auto event = std::move(events.front());
        events.pop();
        return event;
    }

    // Delete copy constructor and assignment operator
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

private:
    std::queue<std::unique_ptr<GameEvent>> events;
};
