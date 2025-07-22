#pragma once;
#include "../System.hpp"
#include "../Components.hpp"

class PlayerSlotSelectionSystem : public System {
public:
    PlayerSlotSelectionSystem() {
        setSignature({
            typeid(PlayerSlot).hash_code(),
            typeid(UIInput).hash_code(),
            typeid(UITransform).hash_code()
        });
    }
    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        for (auto entity : entities) {
            auto& slot = ecs.getComponent<PlayerSlot>(entity);
            auto& input = ecs.getComponent<UIInput>(entity);
            
            if (input.isSelected) {
                slot.playerType = static_cast<PlayerType>((static_cast<int>(slot.playerType) + 1) % 3);
                input.isSelected = false;
            }
        }
    }
};
