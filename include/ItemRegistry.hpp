#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "ECS/Components.hpp"
struct ItemDefinition {
    std::string id;
    std::string name;
    std::string iconPath;
    std::string meshPath;
    
    ItemType type;
    
    bool stackable = true;
    int maxStackSize = 64;
    
};

class ItemRegistry {
public:
    void registerItem(const ItemDefinition& def) {
        items[def.id] = def;
    }

    const ItemDefinition& get(const std::string& id) const {
        auto it = items.find(id);
        if (it == items.end()) throw std::runtime_error("Unknown item ID: " + id);
        return it->second;
    }
    
private:
    std::unordered_map<std::string, ItemDefinition> items;
};
