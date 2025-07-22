#include "GameStateManager.hpp"

void GameStateManager::pushState(std::unique_ptr<GameState> state) {
    if (!states.empty()) {
        states.back()->onExit();
    }
    states.push_back(std::move(state));
    states.back()->onEnter();
}
void GameStateManager::popState() {
    if (!states.empty()) {
        states.back()->onExit();
        states.pop_back();
    }
}
void GameStateManager::replaceState(std::unique_ptr<GameState> state) {
    popState();
    pushState(std::move(state));
}
void GameStateManager::update(float deltaTime) {
    if (!states.empty()) {
        states.back()->update(deltaTime);
    }
}
bool GameStateManager::isEmpty() const {
    return states.empty();
}

void GameStateManager::processEvents() {
    while (eventQueue.hasEvents()) {
        std::unique_ptr<GameEvent> event = eventQueue.pop();

        switch (event -> type) {
        case GameEventType::ChangeState:
            replaceState(std::move(event -> newState));
            break;

        case GameEventType::PushState:
            pushState(std::move(event -> newState));
            break;

        case GameEventType::PopState:
            popState();
            break;

        case GameEventType::ExitApplication:
            // Exit the application by clearing all states
            states.clear();
            break;
        }
    }
}

EventQueue& GameStateManager::getEventQueue() {
    return eventQueue;
}


GameState* GameStateManager::getCurrentState() const {
        if (states.empty()) {
            return nullptr;  // Return nullptr if no states exist
        }
        return states.back().get();
}