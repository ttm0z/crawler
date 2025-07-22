#pragma once
#include <unordered_map>
#include <vector>
#include <bits/algorithmfwd.h>

struct VectorHash {
    template <typename T>
    std::size_t operator ()(const std::vector<T>& v) const {
        std::size_t hash_value = 0;
        for (const auto& element : v) {
            hash_value ^= std::hash<T>{}(element) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);  // FNV-1a hash
        }
        return hash_value;
    }
};
class ArchetypeManager {
public:
    ArchetypeManager() : archetypeEntities() {}

    void assignArchetype(size_t entity, const std::vector<size_t>& componentTypes) {
        archetypeEntities[componentTypes].push_back(entity);
    }

    void updateArchetype(size_t entity, const std::vector<size_t>& newComponentTypes) {
        for (auto& [types, entities] : archetypeEntities) {
            auto it = std::find(entities.begin(), entities.end(), entity);

            if (it != entities.end()) {
                entities.erase(it);
                break;
            }
        }
        assignArchetype(entity, newComponentTypes);
    }

    const std::vector<size_t>& getEntitiesbyArchetype(const std::vector<size_t>& componentTypes) {
        return archetypeEntities[componentTypes];
    }
private:
    std::unordered_map<std::vector<size_t>, std::vector<size_t>, VectorHash> archetypeEntities;
};