#pragma once
#include <vector>
#include <typeindex>
#include "./Components.hpp"
inline std::vector<size_t> uiArchetype = {
    typeid(UIImageElement).hash_code(),
    typeid(UITransform).hash_code(),
};

inline std::vector<size_t> lightArchetype = {
    typeid(LightSourceComponent2D).hash_code()
};
inline std::vector<size_t> physicsArchetype = {
    typeid(PhysicsComponent2D).hash_code(),
    typeid(TransformComponent2D).hash_code(),
};
inline std::vector<size_t> cursorArchetype = {
    typeid(UIImageElement).hash_code(),
    typeid(UITransform).hash_code(),
    typeid(CursorComponent).hash_code()
};
inline std::vector<size_t> cameraArchetype = {
    typeid(CameraComponent2D).hash_code()
};
inline std::vector<size_t> playerArchetype = {
    typeid(PlayerComponent).hash_code(),
    typeid(SpriteComponent).hash_code(),
    typeid(TransformComponent2D).hash_code()
};
inline std::vector<size_t> renderableArchetype = {
    typeid(SpriteComponent).hash_code(),
    typeid(TransformComponent2D).hash_code()
};
inline std::vector<size_t> staticObjectArchetype = {
    typeid(StaticObjectComponent).hash_code()
};
inline std::vector<size_t> timerArchetype = {
    typeid(TimerComponent).hash_code()
};
inline std::vector<size_t> selectionBoxArchetype = {
    typeid(SpriteComponent).hash_code(),
    typeid(SelectionBoxComponent).hash_code()
};
inline std::vector<size_t> populationArchetype = {
    typeid(PopulationUnitComponent).hash_code()
};
inline std::vector<size_t> npcArchetype = {
    typeid(SpriteComponent).hash_code(),
    typeid(TransformComponent2D).hash_code(),
    typeid(PhysicsComponent2D).hash_code(),
    typeid(NPCComponent).hash_code()
};
inline std::vector<size_t> inputArchetype = {
    typeid(UIInput).hash_code(),
    typeid(UITransform).hash_code()
};
inline std::vector<size_t> spriteArchetype = {
    typeid(SpriteComponent).hash_code()
};
inline std::vector<size_t> inventoryArchetype = {
    typeid(InventoryComponent).hash_code()
};
inline std::vector<size_t> dungeonRoomArchetype = {
    typeid(DungeonRoomComponent).hash_code()
};
