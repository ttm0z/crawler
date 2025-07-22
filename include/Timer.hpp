#ifndef TIMER_H
#define TIMER

#include <chrono>
class Timer {
public:
    Timer();
    void start();
    void update();
    float getDeltaTime() const; // Time between frames
    float getElapsedTime() const; // Total elapsed time

    void pause();
    void resume();

private:
    using Clock = std::chrono::high_resolution_clock;

    std::chrono::time_point<Clock> startTime;   // Start time of the timer.
    std::chrono::time_point<Clock> lastFrame;   // Time of the previous frame.
    std::chrono::time_point<Clock> pauseStart;  // When the timer was paused.
    std::chrono::duration<float> pausedTime;    // Total time spent in pause.

    float deltaTime = 0.0f;  // Time between frames.
    bool isPaused = false;
};

#endif