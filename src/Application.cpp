#include "Application.hpp"
#include "tiny_obj_loader.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <Shader.hpp>
#include <GLFW/glfw3.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>


// Global State Management
int running = true;
float lastFrame = 0.0;
float timerInterval = 2.0f; // Timer interval in seconds
float timer = 0.0f;          // Accumulated time

Application::Application()
    : window(1200, 800) { 
    GLFWwindow* glfwWindow = window.getWindow();
    if (!glfwWindow) {
        std::cerr << "GLFW window is not initialized!" << std::endl;
        return;
    }
   
    std::cout << "Application Constructor" << std::endl;
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window.getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetWindowUserPointer(window.getWindow(), this);
    GLfloat maxAniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

}


Application::~Application() {
    cleanup();
}

void Application::init() {}


void Application::processInput(float deltaTime) {
    if(window.isKeyPressed(GLFW_KEY_ESCAPE))
        running = false;

    if(window.isKeyPressed(GLFW_KEY_K)){

    }
        
    if (window.shouldClose()) {
        running = false;
    }
    
}

 // - - - //


// Main Game function
void Application::run() {
    
    Timer timer;
    
    //------------------------
    // Load Global Resources
    //------------------------
    ResourceManager<Shader>::getInstance().load("skyboxShader", "./shaders/skyboxVert.glsl", "./shaders/skyboxFrag.glsl");
    ResourceManager<Shader>::getInstance().load("uiShader", "./shaders/imageVert.glsl", "./shaders/imageFrag.glsl");
    ResourceManager<Shader>::getInstance().load("textShader", "./shaders/textVert.glsl", "./shaders/textFrag.glsl");
    ResourceManager<Shader>::getInstance().load("chunkShader", "./shaders/chunkVert.glsl", "./shaders/chunkFrag.glsl");
    ResourceManager<Shader>::getInstance().load("playerShader", "./shaders/playerVert.glsl", "./shaders/playerFrag.glsl");
    ResourceManager<Shader>::getInstance().load("modelShader", "./shaders/modelVert.glsl", "./shaders/modelFrag.glsl");
    ResourceManager<Shader>::getInstance().load("treeShader", "./shaders/treeShaderVert.glsl", "./shaders/treeShaderFrag.glsl");

    ResourceManager<Texture>::getInstance().load("buttonTexture", "./assets/textures/buttonTexture.png");
    ResourceManager<Texture>::getInstance().load("inventoryBarTexture", "./assets/textures/inventoryBarTexture.png");
    ResourceManager<Texture>::getInstance().load("textBoxTexture", "./assets/textures/textbox_01.png");
    ResourceManager<Texture>::getInstance().load("cursorTexture", "./assets/textures/cursor_01.png");
    ResourceManager<Texture>::getInstance().load("tileTexture", "./assets/textures/cursorTexture.png");
    ResourceManager<Texture>::getInstance().load("playerTexture", "./assets/textures/t1.png");
    ResourceManager<Texture>::getInstance().load("shovelIcon", "./assets/textures/shovel_01.png");
    ResourceManager<Texture>::getInstance().load("handgunIcon", "./assets/textures/handgun_01.png");
    ResourceManager<Texture>::getInstance().load("dirtIcon", "./assets/textures/dirtIcon.png");
    ResourceManager<Texture>::getInstance().load("ammunitionIcon", "./assets/textures/ammunition_01.png");
    ResourceManager<Texture>::getInstance().load("npc_dialog_bg", "./assets/textures/npc_menu_box.png");
    ResourceManager<Texture>::getInstance().load("npc_portrait_default", "./assets/textures/npc_portrait_default.png");
    ResourceManager<Texture>::getInstance().load("inventory_menu_bg", "./assets/textures/inventory_dialog_bg.png");

    std::vector<std::string> chunkArrayFiles = {
    "./assets/textures/water_01.png",
    "./assets/textures/sand_01.png",
    "./assets/textures/grass_01.png",
    "./assets/textures/dirt_01.png",
    "./assets/textures/stone_01.png",
    };
    ResourceManager<Texture>::getInstance().loadTextureArray("chunkArray", chunkArrayFiles);
    
    std::vector<std::string> faces {
    "./assets/skybox/right.jpg",
    "./assets/skybox/left.jpg",
    "./assets/skybox/top.jpg",
    "./assets/skybox/bottom.jpg",
    "./assets/skybox/front.jpg",
    "./assets/skybox/back.jpg"
};

    ResourceManager<Texture>::getInstance().loadCubemap("skyboxTexture", faces);
    
    ResourceManager<Font>::getInstance().load("titleFont", "./assets/fonts/FacultyGlyphic.ttf", 40);
    ResourceManager<Font>::getInstance().load("headerFont", "./assets/fonts/FacultyGlyphic.ttf", 25);
    ResourceManager<Font>::getInstance().load("Faculty-Glyphic", "./assets/fonts/FacultyGlyphic.ttf", 16);
    ResourceManager<Font>::getInstance().load("default", "./assets/fonts/FacultyGlyphic.ttf", 16);

    // Disabled 3D model loading for 2D conversion
    // ResourceManager<Model>::getInstance().load("riggedFigure", "./assets/models/baseModel.glb");
    // ResourceManager<Model>::getInstance().load("woolyMammoth", "./assets/models/mammoth_01.glb");
    // ResourceManager<Model>::getInstance().load("tree1", "./assets/models/Tree1.glb");
    // ResourceManager<Model>::getInstance().load("tree2", "./assets/models/Tree2.glb");
    // ResourceManager<Model>::getInstance().load("tree3", "./assets/models/Tree3.glb");
    // ResourceManager<Model>::getInstance().load("tree4", "./assets/models/Tree4.glb");


    float accumulatedTime = 0.0f;
    auto& eventQueue = GameStateManager::getInstance().getEventQueue();
    GameStateManager::getInstance().pushState(std::make_unique<MainMenuState>(eventQueue));


    while (running) {
        
    //------------------------
    // Update Timer, Process Input
    //------------------------   

        timer.update();
        float deltaTime = timer.getDeltaTime();
        float elapsedTime = timer.getElapsedTime();
        
        // Accumulate the delta time
        accumulatedTime += deltaTime;
        if (accumulatedTime >= 2.0f) {
            //camera.printPosition();

            accumulatedTime = 0.0f;
        }

        processInput(deltaTime);

    //------------------------
    // Render                
    //------------------------    
        
        std::vector<InputEvent> events = window.pollEvents();
        for(InputEvent event : events) {
            InputManager::getInstance().processInput(event);
        }
        
        GameState* currentState = GameStateManager::getInstance().getCurrentState();        
        //update and render the current state
        if (currentState) {
            
            currentState -> update(deltaTime);
        }
        GameStateManager::getInstance().processEvents();
        if(GameStateManager::getInstance().isEmpty()){
            running = false;
        }
        window.update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Application::cleanup() {
    window.destroy();
}

void Application::print(std::string message){
    std::cout << message << std::endl;
}