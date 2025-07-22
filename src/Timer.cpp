    #include "Timer.hpp"
    
    Timer::Timer() { start(); }  // Automatically start the timer upon creation.

    // Start or restart the timer.
    void Timer::start() {
        startTime = std::chrono::high_resolution_clock::now();
        lastFrame = startTime;
        pausedTime = std::chrono::duration<float>(0);  // Reset paused time.
        isPaused = false;
    }

    // Update the timer to calculate delta time between frames.
    void Timer::update() {
        if (isPaused) return;

        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - lastFrame).count();
        lastFrame = currentTime;
    }

    // Pause the timer.
    void Timer::pause() {
        if (!isPaused) {
            pauseStart = std::chrono::high_resolution_clock::now();
            isPaused = true;
        }
    }

    // Resume the timer from a paused state.
    void Timer::resume() {
        if (isPaused) {
            auto pauseEnd = std::chrono::high_resolution_clock::now();
            pausedTime += pauseEnd - pauseStart;
            lastFrame = pauseEnd;  // Reset lastFrame to avoid time jumps.
            isPaused = false;
        }
    }

    // Get the time passed since the timer started (excluding pauses).
    float Timer::getElapsedTime() const {
        auto now = std::chrono::high_resolution_clock::now();
        if (isPaused) {
            return std::chrono::duration<float>(pauseStart - startTime - pausedTime).count();
        }
        return std::chrono::duration<float>(now - startTime - pausedTime).count();
    }

    // Get the delta time (time between the last two frames).
    float Timer::getDeltaTime() const { return deltaTime; }