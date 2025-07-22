#pragma once;
#include "../System.hpp"
#include "../Components.hpp"

class AISystem : public System {
public:
    AISystem() {
        setSignature({
            typeid(AIPlayerComponent).hash_code(),

        });
    }
    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        for (auto entity : entities) {
            auto& aiComponent = ecs.getComponent<AIPlayerComponent>(entity);

    }
};
};