#pragma once
#include "../System.hpp"
#include "../Components.hpp"

// Placeholder AnimationSystem for 2D - disabled for now
class AnimationSystem : public System {
public:
    AnimationSystem() {
        // Empty signature for now
        setSignature({});
    }

    void update(float deltaTime, ECS& ecs) override {
        // Disabled for 2D conversion
    }
};