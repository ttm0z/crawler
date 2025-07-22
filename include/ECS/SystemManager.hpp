#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "System.hpp"
class ECS;
class System;
class SystemManager {
private:
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
    SystemManager() : systems() {}
    template <typename T, typename... Args>
    std::shared_ptr<T> registerSystem(Args&&... args) {
        auto system = std::make_shared<T>(std::forward<Args>(args)...);
        systems[typeid(T)] = system;
        return system;
    }

    template <typename T>
    void setSystemSignature(const std::vector<size_t>& componentTypes) {
        auto system = systems[typeid(T)];
        if (system) {
            system->setSignature(componentTypes); // Pass the reference to setSignature
        }
    }


    void updateSystems(float deltaTime, ECS& ecs) {
        for (auto& [type, system] : systems) {
            system->update(deltaTime, ecs);
        }
    }
};