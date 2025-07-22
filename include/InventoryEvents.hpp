#pragma once
#include <memory>
#include <string>
#include "GameplayEvent.hpp"

struct InventoryAddEvent : public GameplayEvent {
    size_t entity;
    std::string itemId;
    int amount;

    GameplayEventType getType() const override {
        return GameplayEventType::InventoryAdd;
    }
};

struct InventoryRemoveEvent : public GameplayEvent {
    size_t entity;
    std::string itemId;
    int amount;

    GameplayEventType getType() const override {
        return GameplayEventType::InventoryRemove;
    }
};

struct InventorySelectEvent : public GameplayEvent {
    size_t entity;
    std::string itemId;

    GameplayEventType getType() const override {
        return GameplayEventType::InventorySelect;
    }
};
