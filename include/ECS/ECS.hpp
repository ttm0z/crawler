#pragma once

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "ArchetypeManager.hpp"
#include <stddef.h> 
#include <memory>
#include <vector>

class ECS {
public:
    ECS() :
        entityManager(),
        componentManager(),
        systemManager(),
        archetypeManager() {}
    
    // Entity Management
    /** createEntity
     * create a new entity in the entity manager
     * 
     */
    size_t createEntity() { 
        return entityManager.createEntity();
    }
    
    
    /** removeEntity
     *  remove the entity from the system
     */
    void removeEntity(size_t entity) {
        entityManager.destroyEntity(entity);
    }


    // Component Management

    /** addComponent 
     * add a component to an entity
     */
    template <typename T>
    void addComponent(size_t entity, const T& component) {
        componentManager.addComponent(entity, component);
    }

    /** removeComponent
     *  remove a component from an entity
     */
    template <typename T>
    void removeComponent(size_t entity) {
        componentManager.removeComponent<T>(entity);
    }

    /** getComponent
     *  get the component of the specified type for the given entity
     */
    template <typename T>
    T& getComponent(size_t entity) {
        return componentManager.getComponent<T>(entity);
    }
    template<typename T>
    bool hasComponent(size_t entity) {
        return componentManager.hasComponent<T>(entity);
    }

    // System Management

    /** registerSystem
     * registers a new system to the ecs 
     * which operates on entities with the given components
     */
    template <typename T, typename... Args> 
    std::shared_ptr<T> registerSystem(Args&&... args) { 
        return systemManager.registerSystem<T>(std::forward<Args>(args)...);
    };

    template <typename T>
    void setSystemSignature(const std::vector<size_t>& componentTypes) {
        systemManager.setSystemSignature<T>(componentTypes); // Pass reference
    }


     void updateSystems(float deltaTime) {
        systemManager.updateSystems(deltaTime, *this);
     };


     // Archetype Management
     void assignArchetype(size_t entity, const std::vector<size_t>& componentTypes) {
        archetypeManager.assignArchetype(entity, componentTypes);
     }
     void updateArchetype(size_t entity, const std::vector<size_t>& newComponentTypes) {
        archetypeManager.updateArchetype(entity, newComponentTypes);
     }

     const std::vector<size_t>& getEntitiesBySignature(const std::vector<size_t>& componentTypes){
        return archetypeManager.getEntitiesbyArchetype(componentTypes);
     }

private:
    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;
    ArchetypeManager archetypeManager;
};