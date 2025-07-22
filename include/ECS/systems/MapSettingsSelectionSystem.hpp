#pragma once;
#include "../System.hpp"
#include "../Components.hpp"
class MapSettingsSelectionSystem : public System {
public:
    MapSettingsSelectionSystem() {
        setSignature({
            typeid(MapSettings).hash_code(),
            typeid(UIInput).hash_code(),
            typeid(UITransform).hash_code()
        });
    }
    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        for (auto entity : entities) {
            auto& settings = ecs.getComponent<MapSettings>(entity);
            auto& input = ecs.getComponent<UIInput>(entity);

            if (input.isSelected) {
                settings.mapShape = (settings.mapShape == "Hex") ? "Square" : "Hex";
                input.isSelected = false;
            }
        }
    }
};
