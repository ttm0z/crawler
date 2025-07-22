#pragma once;
#include "../System.hpp"
#include "../Components.hpp"

class LightSourceSystem : public System {
public:
    LightSourceSystem() {
        setSignature({
            typeid(LightSourceComponent2D).hash_code()
        });
    }
    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);

        for (auto entity : entities) {
            auto& light = ecs.getComponent<LightSourceComponent2D>(entity);
            //std::cout << "light: " << light.position.y << " \n";
        }
    }
};