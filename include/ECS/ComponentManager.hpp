#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>

class ComponentManager {
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> componentPools;

public:
    ComponentManager() {}
    template<typename T>
    void addComponent(size_t entity, const T& component) {
        std::type_index typeIndex(typeid(T));

        if (componentPools.find(typeIndex) == componentPools.end()) {
            componentPools[typeIndex] = std::make_shared<std::vector<T>>();
        }

        auto& pool = *std::static_pointer_cast<std::vector<T>>(componentPools[typeIndex]);
        if(entity >= pool.size()) {
            pool.resize(entity+1);
        }
        pool[entity] = component;
    }
    
    template<typename T>
    void removeComponent(size_t entity) {
        std::type_index typeIndex(typeid(T));

        if (componentPools.find(typeIndex) == componentPools.end()) return;

        auto& pool = *std::static_pointer_cast<std::vector<T>>(componentPools[typeIndex]);
        if (entity < pool.size()) {
            pool[entity] = T();
        }
    }
    template <typename T>
    bool hasComponent(size_t entity) {
        std::type_index typeIndex(typeid(T));

        if (componentPools.find(typeIndex) == componentPools.end()) return false;

        auto& pool = *std::static_pointer_cast<std::vector<T>>(componentPools[typeIndex]);
        return entity < pool.size();
    }

    template <typename T>
    T& getComponent(size_t entity) {
        std::type_index typeIndex(typeid(T));

        // Ensure the component pool exists
        if (componentPools.find(typeIndex) == componentPools.end()) {
            throw std::out_of_range("Component pool for this type doesn't exist!");
        }

        // Get the pool and return the component
        auto& pool = *std::static_pointer_cast<std::vector<T>>(componentPools[typeIndex]);
        if (entity >= pool.size()) {
            throw std::out_of_range("Entity does not have this component!");
        }
        return pool[entity];
    }
};
