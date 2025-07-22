#include "Window.hpp"




std::vector<InputEvent> Window::eventBuffer; 
Window::Window(int width, int height)
    : width(width), height(height), title("OpenGL Window"), window(nullptr), mousePosX(0.0), mousePosY(0.0) {
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Set GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    std::cout << "GLFW Initialized" << std::endl;

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetWindowUserPointer(window, this);
    

    // Initialize GLAD after creating the OpenGL context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
    std::cout << "GLAD Initialized" << std::endl;

            // Register GLFW callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        auto* wnd = static_cast<Window*>(glfwGetWindowUserPointer(win));
        wnd->handleKeyEvent(key, action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* wnd = static_cast<Window*>(glfwGetWindowUserPointer(win));
        wnd->handleMouseMove(xpos, ypos);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double xOffset, double yOffset){
        auto* wnd = static_cast<Window*>(glfwGetWindowUserPointer(win));
        wnd->handleScroll(xOffset, yOffset);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        auto* wnd = static_cast<Window*>(glfwGetWindowUserPointer(win));
        wnd->handleMouseButton(button, action);
    });

    // Set viewport size
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // Enable VSync
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f); 
}

Window::~Window() {
    destroy();
}

void Window::update() {
    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Update mouse position
    //glfwGetCursorPos(window, &mousePosX, &mousePosY);

    clear();
}

void Window::clear() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

std::vector<InputEvent> Window::pollEvents() {
    glfwPollEvents();
    std::vector<InputEvent> events;

    events.swap(eventBuffer); // Move events out of the buffer
    return events;
}

std::vector<int> Window::getFramebufferCoordinates(){
    int f_width;
    int f_height;
    glfwGetFramebufferSize(window, &f_width, &f_height);
    return std::vector{f_width, f_height};
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

GLFWwindow* Window::getWindow() {
    return window;
}

void Window::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool Window::isKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

double Window::getMousePosX() {
    return mousePosX;
}

double Window::getMousePosY() {
    return mousePosY;
}

void Window::destroy() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}
