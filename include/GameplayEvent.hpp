#pragma once
#include <memory>
enum class GameplayEventType {
    InventoryAdd,
    InventoryRemove,
    InventorySelect,
    // Extend for other systems
};

struct GameplayEvent {
    virtual ~GameplayEvent() = default;
    virtual GameplayEventType getType() const = 0;
};