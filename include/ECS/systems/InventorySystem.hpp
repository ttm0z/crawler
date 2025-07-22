#pragma once
#include <string>
#include <algorithm>
#include "../System.hpp"
#include "../Components.hpp"
#include "../../GameplayEventQueue.hpp"
#include "../../InventoryEvents.hpp" 

class InventorySystem : public System {
    
public:
    ItemRegistry* itemRegistry = nullptr;
    size_t inventoryBar;
    InventorySystem() {
        setSignature({
            typeid(InventoryComponent).hash_code()
        });
    }


    void update(float deltaTime, ECS& ecs) override {
        auto entities = ecs.getEntitiesBySignature(signature);
        auto& inventoryBarComponent = ecs.getComponent<InventoryBarComponent>(inventoryBar);
        for(auto entity : entities){
            auto& inventoryComponent = ecs.getComponent<InventoryComponent>(entity);
            updateInventoryBar(&ecs, inventoryBarComponent, inventoryComponent, *itemRegistry);
        }
        

        if (!eventQueue) return;
        // Process all events
        while (eventQueue->hasEvents()) {
            auto event = eventQueue->pop();
            switch (event->getType()) {
                case GameplayEventType::InventoryAdd: {
                    auto* addEvent = static_cast<InventoryAddEvent*>(event.get());
                    GameItemComponent item;
                    item.itemID = addEvent->itemId;
                    item.quantity = addEvent->amount;
                    addItem(ecs, addEvent->entity, item);
                    break;
                }

                case GameplayEventType::InventoryRemove: {
                    auto* removeEvent = static_cast<InventoryRemoveEvent*>(event.get());
                    removeItem(ecs, removeEvent->entity, removeEvent->itemId, removeEvent->amount);
                    break;
                }

                case GameplayEventType::InventorySelect: {
                    auto* selectEvent = static_cast<InventorySelectEvent*>(event.get());
                    auto& inventory = ecs.getComponent<InventoryComponent>(selectEvent->entity);
                    updateSelected(inventory, selectEvent->itemId);
                    break;
                }

                default:
                    break;
            }
        }

        // Optional per-frame inventory logic
        auto inventories = ecs.getEntitiesBySignature(signature);
        for (auto entity : inventories) {
            auto& inventory = ecs.getComponent<InventoryComponent>(entity);
            // Future logic like cooldowns or animations
        }
    }

private:

    void updateSelected(InventoryComponent& inventory, const std::string& selected) {
        inventory.selected = selected;
    }
    
    void addItem(ECS& ecs, size_t entity, const GameItemComponent& newItem) {
        auto& inventory = ecs.getComponent<InventoryComponent>(entity);
    
        if (inventory.items.size() >= inventory.maxSlots) {
            std::cout << "Inventory full!\n";
            return;
        }
    
        for (auto& item : inventory.items) {
            if (item.itemID == newItem.itemID) {
                item.quantity += newItem.quantity;
                
                // Print inventory contents
                std::cout << "Inventory after adding " << newItem.itemID << ":\n";
                for (const auto& i : inventory.items) {
                    std::cout << " - " << i.itemID << " x" << i.quantity << "\n";
                }
    
                return;
            }
        }
    
        inventory.items.push_back(newItem);
    
        // Print inventory contents
        std::cout << "Inventory after adding new item " << newItem.itemID << ":\n";
        for (const auto& i : inventory.items) {
            std::cout << " - " << i.itemID << " x" << i.quantity << "\n";
        }
    }

    bool removeItem(ECS& ecs, size_t entity, const std::string& itemId, int quantity) {
        auto& inventory = ecs.getComponent<InventoryComponent>(entity);

        for (auto it = inventory.items.begin(); it != inventory.items.end(); ++it) {
            if (it->itemID == itemId) {
                if (it->quantity < quantity) return false;

                it->quantity -= quantity;
                if (it->quantity == 0) {
                    inventory.items.erase(it);
                }
                return true;
            }
        }
        return false;
    }

public:
    bool hasItem(ECS& ecs, size_t entity, const std::string& itemId, int quantity) const {
        const auto& inventory = ecs.getComponent<InventoryComponent>(entity);
        for (const auto& item : inventory.items) {
            if (item.itemID == itemId && item.quantity >= quantity) {
                return true;
            }
        }
        return false;
    }

    int getItemCount(ECS& ecs, size_t entity, const std::string& itemId) const {
        const auto& inventory = ecs.getComponent<InventoryComponent>(entity);
        for (const auto& item : inventory.items) {
            if (item.itemID == itemId) {
                return item.quantity;
            }
        }
        return 0;
    }
};
