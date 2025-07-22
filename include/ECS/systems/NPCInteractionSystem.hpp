#pragma once;
#include "../System.hpp"
#include "../Components.hpp"
#include "../../util/Render.hpp"
#include <unordered_set>
#include <unordered_map>

class NPCInteractionSystem : public System {

public:

    NPCInteractionSystem() {
        setSignature({
            typeid(PlayerComponent).hash_code()
        });
    }
void update(float deltaTime, ECS& ecs) override {
    auto entities = ecs.getEntitiesBySignature(signature);


    


    for (auto entity : entities) {}
}
};