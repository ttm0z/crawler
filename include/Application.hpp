#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.hpp"
#include "Timer.hpp"
#include "ECS/ECS.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include "GameStates.hpp"

class Application{
public:
    Application();
    ~Application();
    
    void init();
    void run();
    void cleanup();

    void processInput(float deltaTime);
    
private:
    Window window;
    void print(std::string message);
};

#endif