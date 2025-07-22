#pragma once;
#include "../System.hpp"
#include "../../util/Render.hpp"
#include "../Components.hpp"
#include "../../InputManager.hpp"
#include "../../InventoryEvents.hpp"
#include "ChunkSystem.hpp"

class PlayerSystem : public System {
public:
    size_t cameraEntity;
    size_t textBox;
    size_t selectedNPC;
    size_t currentPlayer;
    NPCInteractionElements interactionMenu;
    InventoryMenuElements inventoryMenu;
    ItemRegistry* itemRegistry = nullptr;
    bool menuExists = false;
    bool showMenu = false;
    ChunkSystem* chunkSystem;
    float playerCameraHeight = 3.0f;
    float playerMovementSpeed = 0.1f;
    ECS* ecs = nullptr;
    PlayerSystem() {
        setSignature({
            typeid(PlayerComponent).hash_code(),
            typeid(RenderableComponent).hash_code(),
            typeid(TransformComponent).hash_code()

        });
        InputManager::getInstance().subscribe(InputEventType::MOUSE_BUTTON_LEFT, [this](const InputEvent& event) {
            handleClick(this -> ecs, event.type, event.mouseX, event.mouseY);
        });
        InputManager::getInstance().subscribe(InputEventType::MOUSE_BUTTON_RIGHT, [this](const InputEvent& event) {
            handleClick(this -> ecs, event.type, event.mouseX, event.mouseY);
        });
        InputManager::getInstance().subscribe(InputEventType::MOUSE_BUTTON_LEFT_RELEASE, [this](const InputEvent& event) {
            handleClick(this -> ecs, event.type, event.mouseX, event.mouseY);
        });
        InputManager::getInstance().subscribe(InputEventType::MOUSE_BUTTON_RIGHT_RELEASE, [this](const InputEvent& event) {
            handleClick(this -> ecs, event.type, event.mouseX, event.mouseY);
        });
    }
void update(float deltaTime, ECS& ecs) override {
    auto entities = ecs.getEntitiesBySignature(signature);
    auto& camera = ecs.getComponent<CameraComponent3D>(cameraEntity);

    for (auto entity : entities) {
        
        currentPlayer = entity;
        auto& transform = ecs.getComponent<TransformComponent>(entity);
        auto& player = ecs.getComponent<PlayerComponent>(entity);
        auto& physics = ecs.getComponent<PhysicsComponent2D>(entity);
        auto& inventory = ecs.getComponent<InventoryComponent>(entity);
        auto& animation = ecs.getComponent<AnimationComponent>(entity);
        
        

        if (player.isWalking){
            animation.currentAnimation = "Walk_01";
        }
        else animation.currentAnimation = "Idle_01";
    
    
    
    auto gameState = ecs.getEntitiesBySignature({typeid(GameStateComponent).hash_code()});
    auto& gameStateComponent = ecs.getComponent<GameStateComponent>(gameState[0]);
    // Handle NPCInteractionMenu        
    
    
    if(!showMenu && selectedNPC && InputManager::getInstance().isKeyPressed(GLFW_KEY_E))
        showMenu = true;

    
    if(showMenu && selectedNPC){
        auto& npcComponent = ecs.getComponent<NPCComponent>(selectedNPC);
        if (!menuExists){
            interactionMenu = createNPCInteractionMenu(ecs, npcComponent);
            menuExists = true;
        }
            
        auto gameState = ecs.getEntitiesBySignature({typeid(GameStateComponent).hash_code()});
        auto& gameStateComponent = ecs.getComponent<GameStateComponent>(gameState[0]);
        gameStateComponent.isGameplayFrozen = true;
    }
    
    // Close the NPCInteractionMenu
    if (!selectedNPC || InputManager::getInstance().isKeyPressed(GLFW_KEY_SEMICOLON)) {
            showMenu = false;
            menuExists = false;
            setNPCInteractionMenuVisible(ecs, interactionMenu, false);
            gameStateComponent.isGameplayFrozen = false;
    }
    
    
    //Handle InventoryMenu
    if (!showMenu && InputManager::getInstance().isKeyPressed(GLFW_KEY_I)){
        showMenu = true;
        if (!menuExists){
            inventoryMenu = createInventoryMenu(ecs, inventory, *itemRegistry);
            menuExists = true;
        }
        gameStateComponent.isGameplayFrozen = true;
    }

    // Close the InventoryMenu
if (!showMenu && InputManager::getInstance().isKeyPressed(GLFW_KEY_SEMICOLON)) {
            showMenu = false;
            menuExists = false;
            setInventoryMenuVisible(ecs, inventoryMenu, false);
            gameStateComponent.isGameplayFrozen = false;
    }
    
    if(gameStateComponent.isGameplayFrozen) return;        
    // Handle Player Input
        player.isWalking = false;
        player.direction.x = cos(transform.rotation.x) * sin(transform.rotation.y);
        player.direction.y = sin(transform.rotation.x);
        player.direction.z = cos(transform.rotation.x) * cos(transform.rotation.y);
        player.direction = glm::normalize(player.direction) * -1.0f;
        player.right = glm::normalize(glm::cross(player.direction, glm::vec3(0.0f, 1.0f, 0.0f)));

        // Handle rotation with A/D keys
        const float rotationSpeed = glm::radians(90.0f);
        
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_A)) {
            transform.rotation.y += rotationSpeed * deltaTime;
            camera.yaw -= 90.0f * deltaTime;
            player.isWalking = true;
        }
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_D)) {
            transform.rotation.y -= rotationSpeed * deltaTime;
            camera.yaw += 90.0f * deltaTime;
            player.isWalking = true;
        }

        // Apply movement forces (instead of setting velocity directly)
        glm::vec2 movement(0.0f);
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_W)) {
            movement -= glm::vec2(player.direction.x, player.direction.z) * player.movementSpeed;
            player.isWalking = true;
        }
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_S)) {
            movement += glm::vec2(player.direction.x, player.direction.z) * player.movementSpeed;
            player.isWalking = true;
        }
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_Q)) {
            physics.velocity.y = 1;
            movement.y += player.movementSpeed; // 2D up movement
        }
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_E)) {
            movement.y -= player.movementSpeed; // 2D down movement
        }
        if (InputManager::getInstance().isKeyPressed(GLFW_KEY_SPACE)) { // Removed onGround check for 2D
            physics.velocity.y = player.movementSpeed; // Jump force
            player.isWalking = false;
        }
        // Apply input movement as acceleration instead of velocity
        if(glm::length(physics.velocity) < player.movementSpeed)
            physics.acceleration += movement;
        
    }
}
void setNPCInteractionMenuVisible(ECS& ecs, NPCInteractionElements& menu, bool visible) {
    auto setVisibility = [&](size_t entity) {
        if (ecs.hasComponent<UITextElement>(entity)) {
            
            ecs.getComponent<UITextElement>(entity).isTextVisible = visible;
            
        }
        if (ecs.hasComponent<UIImageElement>(entity)) {
            ecs.getComponent<UIImageElement>(entity).isImageVisible = visible;
        }
    };

    setVisibility(menu.panel);
    setVisibility(menu.headerText);
    setVisibility(menu.levelText);
    setVisibility(menu.npcPortrait);
    setVisibility(menu.npcDialogue);
    setVisibility(menu.playerDialogue);
}
void setInventoryMenuVisible(ECS& ecs, InventoryMenuElements& menu, bool visible) {
    auto setVisibility = [&](size_t entity) {
        if (ecs.hasComponent<UITextElement>(entity)) {
            ecs.getComponent<UITextElement>(entity).isTextVisible = visible;
        }
        if (ecs.hasComponent<UIImageElement>(entity)) {
            ecs.getComponent<UIImageElement>(entity).isImageVisible = visible;
        }
    };

    setVisibility(menu.panel);
    setVisibility(menu.headerText);
    
    for (auto entity : menu.inventoryList) {
        setVisibility(entity);
    }

    for (auto entity : menu.craftingList) {
        setVisibility(entity);
    }

    setVisibility(menu.selectedItemDescription);
    setVisibility(menu.craftButton);
}

void handleClick(ECS* ecs, InputEventType type, float screenX, float screenY) {
    

    auto& camera = ecs->getComponent<CameraComponent3D>(cameraEntity);

    glm::vec3 rayOrigin = camera.position;
    glm::vec3 rayDir = camera.direction;

    const float maxDistance = 100.0f;
    const float stepSize = 0.1f;

    glm::vec3 currentPos = rayOrigin;

    for (float traveled = 0.0f; traveled < maxDistance; traveled += stepSize) {
        currentPos = rayOrigin + rayDir * traveled;

        auto npcEntities = ecs->getEntitiesBySignature(npcArchetype);
        size_t closestNPC;
        float closestHit = std::numeric_limits<float>::max();

        for (auto npcEntity : npcEntities) {
            auto& npcComponent = ecs->getComponent<NPCComponent>(npcEntity);
            auto& npcTransform = ecs->getComponent<TransformComponent>(npcEntity);
            auto& npcCollider = ecs->getComponent<ColliderComponent2D>(npcEntity);

            // Generate AABB in world space
            glm::vec2 halfExtents = npcCollider.size;
            glm::vec3 aabbMin = npcTransform.position - glm::vec3(halfExtents.x, halfExtents.y, 0);
            glm::vec3 aabbMax = npcTransform.position + glm::vec3(halfExtents.x, halfExtents.y, 0);

            float tHit;
            
            if (rayIntersectsAABB(rayOrigin, rayDir, aabbMin, aabbMax, tHit)) {
                if (tHit < closestHit) {
                    closestHit = tHit;
                    selectedNPC = npcEntity;
                    std::string attitude = "";
                    if (npcComponent.attitude == NPCState::Friendly){
                        attitude = "Friendly";
                    }
                    if (npcComponent.attitude == NPCState::Neutral){
                        attitude = "Neutral";
                    }
                    if (npcComponent.attitude == NPCState::Hostile){
                        attitude = "Hostile";
                    }
                    auto& textbox = ecs->getComponent<TextBoxComponent>(textBox);
                    textbox.text =  npcComponent.name + "\n" +
                    "Level: " + std::to_string(npcComponent.level) + "\n" +
                    attitude + "\n";
                    return;
                }
            }
        }


        //check for occupied tile
        glm::ivec2 chunkCoord = worldToChunkCoord(currentPos, 16, 1.0f);
        //std::cout << "[PlayerSystem::handleClick] " << chunkCoord.x << " " << chunkCoord.y << "\n";
        size_t chunkEntity = chunkSystem -> getChunkEntity({chunkCoord.x, chunkCoord.y, 0});

        if (!ecs->hasComponent<ChunkComponent>(chunkEntity)) continue;

        auto& chunk = ecs->getComponent<ChunkComponent>(chunkEntity);
        glm::ivec3 tileCoord = worldToLocalTileCoord(currentPos, chunkCoord, 16, 1.0f);

        // Safety check bounds
        if (tileCoord.x < 0 || tileCoord.y < 0 || tileCoord.z < 0 ||
            tileCoord.x >= 16 || tileCoord.y >= 16 || tileCoord.z >= 40) {
            continue;
        }

        int tileIndex = chunk.indexFrom3D(tileCoord.x, tileCoord.y, tileCoord.z);

        if (chunk.occupancy[tileIndex] == 1) {
            if (type == InputEventType::MOUSE_BUTTON_LEFT_RELEASE){
                if(ecs->hasComponent<TextBoxComponent>(textBox)){
                    auto& textbox = ecs->getComponent<TextBoxComponent>(textBox);
                    textbox.text = "Selected Tile: (" + 
                                    std::to_string(tileCoord.x + chunkCoord.x*16) + ", " +
                                    std::to_string(tileCoord.y + chunkCoord.y*16) + ", " +
                                    std::to_string(tileCoord.z) + ")\n";
                    std::cout << textbox.text << "\n";
                }
            }
            if (type == InputEventType::MOUSE_BUTTON_RIGHT_RELEASE){
                chunk.dirty = true;
                chunk.occupancy[tileIndex] = 0;
                // Construct and push InventoryAddEvent
                auto addEvent = std::make_unique<InventoryAddEvent>();
                addEvent->entity = (ecs->getComponent<PlayerComponent>(currentPlayer)).inventoryID;
                addEvent->itemId = "dirt";
                addEvent->amount = 1;
                std::cout << "adding one dirt to inventory . . . \n";
                (*eventQueue).push(std::move(addEvent));
            }
            return;
        }
    }

    std::cout << "[PlayerSystem]: No occupied tile found along ray.\n";
}

bool rayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
                       const glm::vec3& aabbMin, const glm::vec3& aabbMax,
                       float& tNear) {
    const float EPSILON = 1e-6f;
    float tMin = -INFINITY;
    float tMax = INFINITY;

    for (int i = 0; i < 3; ++i) {
        float origin = rayOrigin[i];
        float direction = rayDir[i];
        float minBound = aabbMin[i];
        float maxBound = aabbMax[i];

        if (fabs(direction) < EPSILON) {
            // Ray is parallel to slab. No hit if origin not within slab.
            if (origin < minBound || origin > maxBound) {
                return false;
            }
        } else {
            float t1 = (minBound - origin) / direction;
            float t2 = (maxBound - origin) / direction;

            if (t1 > t2) std::swap(t1, t2);

            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax) return false;
        }
    }

    tNear = tMin;
    return tNear >= 0.0f;
}


glm::vec2 screenToNDC(float mouseX, float mouseY, float screenWidth, float screenHeight) {
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    return glm::vec2(x, y);
}
glm::vec3 screenPosToWorldRay(
    float mouseX, float mouseY, 
    float screenWidth, float screenHeight,
    const glm::mat4& projection, 
    const glm::mat4& view
) {
    glm::vec4 rayClip = glm::vec4(screenToNDC(mouseX, mouseY, screenWidth, screenHeight), -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
    return rayWorld;
}
bool intersectRayPlane(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    const glm::vec3& planeNormal,
    float planeD,
    glm::vec3& intersectionPoint
) {
    float denom = glm::dot(planeNormal, rayDir);
    if (abs(denom) < 1e-6) return false; // parallel to plane

    float t = -(glm::dot(planeNormal, rayOrigin) + planeD) / denom;
    

    intersectionPoint = rayOrigin + t * rayDir;
    return true;
}
glm::ivec2 pickTileAtPoint(glm::vec3 intersection, int chunkSize, float hexRadius) {
    glm::ivec2 chunk = worldToChunkCoord(intersection, chunkSize, hexRadius);
    glm::ivec3 localTile = worldToLocalTileCoord(intersection, chunk, chunkSize, hexRadius);
    return glm::ivec2(chunk.x * chunkSize + localTile.x, chunk.y * chunkSize + localTile.y);
}

bool intersectRayHexPrism(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    const glm::vec3& tileCenter,
    float hexRadius,
    float tileHeight,
    float& t
) {
    // Intersection with top and bottom planes
    float tTop, tBottom;
    glm::vec3 normal(0, 1, 0);

    // Bottom plane
    float dBottom = -tileCenter.y;
    if (!intersectRayPlane(rayOrigin, rayDir, normal, dBottom, tBottom)) return false;

    glm::vec3 pBottom = rayOrigin + rayDir * tBottom;
    if(pBottom.y < tileCenter.y || pBottom.y > tileCenter.y + tileHeight) return false;

    if(!isPointInHexagon(pBottom, tileCenter, hexRadius)) return false;

    t = tBottom;
    return true;
}

bool intersectRayPlane(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    const glm::vec3& planeNormal,
    float planeD,
    float& t
) {
    float denom = glm::dot(planeNormal, rayDir);
    if (abs(denom) < 1e-6) return false;

    t = -(glm::dot(planeNormal, rayOrigin) + planeD) / denom;
    return t >= 0.0f;
}

bool isPointInHexagon(const glm::vec3& p, const glm::vec3& center, float hexRadius) {
    // Convert to local hex coordinates (flat-topped hex)
    glm::vec2 local(p.x - center.x, p.z - center.z);
    local = glm::abs(local);
    float q = (local.x * sqrt(3)/3 - local.y / 3);
    float r = local.y * 2.f/3;

    return glm::length(glm::vec2(q,r)) <= hexRadius * 0.95f; // slightly smaller for safety
}

};