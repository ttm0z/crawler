#pragma once
#include "GameState.hpp"
#include "ItemRegistry.hpp"
#include <string>
#include <iomanip>
#include <memory>
#include "ResourceManager.hpp"
#include "util/Render.hpp"
#include "ECS/systems/UISystem.hpp"
#include "ECS/systems/CameraSystem2D.hpp"
#include "ECS/systems/CameraSystem3D.hpp"
#include "ECS/systems/RenderSystem.hpp"
#include "ECS/systems/UIInputSystem.hpp"
#include "ECS/systems/CursorSystem.hpp"
#include "ECS/systems/ChunkSystem.hpp"
#include "ECS/systems/PlayerSystem.hpp"
#include "ECS/systems/PhysicsSystem.hpp"
#include "ECS/systems/NPCSystem.hpp"
#include "ECS/systems/AnimationSystem.hpp"
#include "ECS/systems/LightSourceSystem.hpp"
#include "ECS/systems/InventorySystem.hpp"
#include "ECS/Archetypes.hpp"
#include "WorldGenerator.hpp"
#include "ECS/systems/PlayerSlotSelectionSystem.hpp"
#include "ECS/systems/MapSettingsSelectionSystem.hpp"
#include "./GameplayEventQueue.hpp"


class GamePlayState : public GameState {
public:
    ItemRegistry itemRegistry;
    // std::shared_ptr<Model> testModel = nullptr; // Disabled for 2D conversion
    explicit GamePlayState(EventQueue& eventQueue) : GameState(eventQueue) {}
    void onEnter() override {
        init();
    }
    void update(float deltaTime) override {

        ecs -> updateSystems(deltaTime);
    }
private:
    GameplayEventQueue* m_eventQueue = nullptr;
    void init(){
        std::cout << "Init Gameplay state . . ." << "\n";
        registerItems();
        m_eventQueue = new GameplayEventQueue;
        // Register Systems
        //auto turnSystem = ecs -> registerSystem<TurnSystem>();
        //auto cursorSystem = ecs -> registerSystem<CursorSystem>();
        auto uiSystem = ecs -> registerSystem<UISystem>();
        auto cursorSystem = ecs -> registerSystem<CursorSystem>();
        auto cameraSystem = ecs -> registerSystem<CameraSystem3D>();
        auto cameraSystem2D = ecs -> registerSystem<CameraSystem2D>();
        auto chunkSystem = ecs -> registerSystem<ChunkSystem>();
        auto renderSystem = ecs -> registerSystem<RenderSystem>();
        auto playerSystem = ecs -> registerSystem<PlayerSystem>();        
        auto physicsSystem = ecs -> registerSystem<PhysicsSystem>();
        auto npcSystem = ecs -> registerSystem<NPCSystem>();
        auto animationSystem = ecs -> registerSystem<AnimationSystem>();
        auto lightSystem = ecs -> registerSystem<LightSourceSystem>();
        auto inventorySystem = ecs -> registerSystem<InventorySystem>();
        //auto playerSystem = ecs -> registerSystem<PlayerSystem>();
        //tileInputSystem -> ecs = ecs.get();
        cameraSystem -> ecs = ecs.get();
        

        // Set System Signatures
        ecs -> setSystemSignature<CameraSystem3D>(cameraArchetype);
        ecs -> setSystemSignature<RenderSystem>(renderableArchetype);
        ecs -> setSystemSignature<PlayerSystem>(playerArchetype);
        ecs -> setSystemSignature<ChunkSystem>({typeid(ChunkComponent).hash_code()});
        ecs -> setSystemSignature<CameraSystem2D>({typeid(CameraComponent2D).hash_code()});
        ecs -> setSystemSignature<UISystem>(uiArchetype);
        ecs -> setSystemSignature<PhysicsSystem>(physicsArchetype);
        ecs -> setSystemSignature<CursorSystem>(cursorArchetype);
        ecs -> setSystemSignature<NPCSystem>(npcArchetype);
        ecs -> setSystemSignature<InventorySystem>(inventoryArchetype);


        size_t gameState = ecs -> createEntity();
        ecs->assignArchetype(gameState, {typeid(GameStateComponent).hash_code()});
        GameStateComponent gameStateComponent;
        ecs -> addComponent(gameState, gameStateComponent);
        
        // skybox
        auto skyboxEntity = ecs -> createEntity();
        ecs -> assignArchetype(skyboxEntity, renderableArchetype);
        // ecs -> assignArchetype(skyboxEntity, skyboxArchetype); // Disabled for 2D conversion
        RenderableComponent skyboxRenderable;
        TransformComponent skyboxTransform;
        SkyboxComponent skyboxComponent;
        skyboxComponent.skyboxShader = "skyboxShader";
        skyboxComponent.cubemapId = "skyboxTexture";
        initializeSkyboxRenderable(skyboxRenderable, skyboxComponent);
        ecs -> addComponent(skyboxEntity, skyboxRenderable);
        ecs -> addComponent(skyboxEntity, skyboxTransform);
        ecs -> addComponent(skyboxEntity, skyboxComponent);
        
        //3d camera
        auto cameraEntity = ecs -> createEntity();
        ecs -> assignArchetype(cameraEntity, cameraArchetype);
        CameraComponent3D cameraComponent3D;
        ecs -> addComponent(cameraEntity, cameraComponent3D);

        //2d camera
        auto camera2D = ecs -> createEntity();
        ecs -> assignArchetype(camera2D, {typeid(CameraComponent2D).hash_code()});
        CameraComponent2D cameraComponent2D;
        ecs -> addComponent(camera2D, cameraComponent2D);
        
        // Generate world
        WorldGenerator generator(0);
        auto worldMap = generator.generateWorld(20, 20);
        generator.printWorld(worldMap);
        generator.printElevationMap(worldMap);
        glm::ivec2 startingChunk = {0, 0};
        for (int i = 0; i < worldMap.size(); i++){
            for (int j = 0; j < worldMap[0].size(); j++) {
                if (worldMap[i][j].baseElevation == 1) {//ocean
            }
        }
    }
        // UI Element : World Coordinates display
        size_t worldCoordinates = ecs -> createEntity();
        ecs -> assignArchetype(worldCoordinates, uiArchetype);
        UITextElement startTextElement("Coordinates: ", "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, true);
        UIImageElement startImageElement("buttonTexture", false);
        UITransform startTransform(glm::vec3(100.0f, 300.0f, 0.0f), glm::vec2(120.0f, 120.0f), glm::vec2(1.0f, 1.0f));
        UIInput startInput(
            [](){},
            [](){}
        );
        ecs -> addComponent(worldCoordinates, startTextElement);
        ecs -> addComponent(worldCoordinates, startImageElement);
        ecs -> addComponent(worldCoordinates, startTransform);
        ecs -> addComponent(worldCoordinates, startInput);

        // UI Element : Tile Coordinates display
        size_t tileCoordinates = ecs -> createEntity();
        ecs -> assignArchetype(tileCoordinates, uiArchetype);
        UITextElement tileCoordinatesTextElement("Tile Coordinates: ", "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, true);
        UIImageElement tileCoordinatesImageElement("buttonTexture", false);
        UITransform tileCoordinatesTransform(glm::vec3(100.0f, 200.0f, 0.0f), glm::vec2(120.0f, 120.0f), glm::vec2(1.0f, 1.0f));
        UIInput tileCoordinatesInput(
            [](){},
            [](){}
        );
        ecs -> addComponent(tileCoordinates, tileCoordinatesImageElement);
        ecs -> addComponent(tileCoordinates, tileCoordinatesTextElement);
        ecs -> addComponent(tileCoordinates, tileCoordinatesTransform);
        ecs -> addComponent(tileCoordinates, tileCoordinatesInput);

        // UI Element : Chunk Coordinates display
        size_t chunkCoordinates = ecs -> createEntity();
        ecs -> assignArchetype(chunkCoordinates, uiArchetype);
        UITextElement chunkTextElement("Chunk Coordinates: ", "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, true);
        UIImageElement chunkImageElement("buttonTexture", false);
        UITransform chunkTransform(glm::vec3(100.0f, 100.0f, 0.0f), glm::vec2(120.0f, 120.0f), glm::vec2(1.0f, 1.0f));
        UIInput chunkInput(
            [](){},
            [](){}
        );
        ecs -> addComponent(chunkCoordinates, chunkTextElement);
        ecs -> addComponent(chunkCoordinates, chunkImageElement);
        ecs -> addComponent(chunkCoordinates, chunkTransform);
        ecs -> addComponent(chunkCoordinates, chunkInput);


        // Inventory Bar
        size_t textBox = ecs -> createEntity();
        ecs -> assignArchetype(textBox, uiArchetype);
        UITextElement textBoxTextElement("", "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 16.0f, true);
        UIImageElement textBoxImageElement("textBoxTexture", true);
        UITransform textBoxTransform(glm::vec3(1000.0f, 200.0f, -0.1f), glm::vec2(300.0f, 100.0f), glm::vec2(1.0f, 1.0f));
        TextBoxComponent textBoxComponent;
        UIInput textBoxInput(
            [](){},
            [](){}
        );
        ecs -> addComponent(textBox, textBoxTextElement);
        ecs -> addComponent(textBox, textBoxImageElement);
        ecs -> addComponent(textBox, textBoxTransform);
        ecs -> addComponent(textBox, textBoxInput);
        ecs -> addComponent(textBox, textBoxComponent);

        // Cursor Entity
        size_t cursorEntity = ecs -> createEntity();
        ecs -> assignArchetype(cursorEntity, cursorArchetype);
        ecs -> assignArchetype(cursorEntity, uiArchetype);
        UITextElement cursorText("", "Faculty-Glyphic", glm::vec3(0.0f, 0.0f, 0.0f), 16.0f, false);
        UIImageElement cursorElement("cursorTexture", true);
        UITransform cursorTransform(glm::vec3(300.0f, 600.0f, 0.1f), glm::vec2(25.0f, 25.0f), glm::vec2(1.0f, 1.0f));
        
        CursorComponent cursorComponent(300.0f, 600.0f, 0.0f, 0.0f, 0.50f, true);
        cursorComponent.is3D = true;
        ecs -> addComponent(cursorEntity, cursorElement);
        ecs -> addComponent(cursorEntity, cursorTransform);
        ecs -> addComponent(cursorEntity, cursorComponent);
        ecs -> addComponent(cursorEntity, cursorText);


        // create trees

        auto tree1 = addStaticObject(ecs.get(), glm::vec3(0.0f, 5.0f, 0.0f), StaticObject::TREE);
        auto tree2 = addStaticObject(ecs.get(), glm::vec3(15.0f, 5.0f, 0.0f), StaticObject::TREE);
        auto tree3 = addStaticObject(ecs.get(), glm::vec3(15.0f, 5.0f, -15.0f), StaticObject::TREE);
        auto tree4 = addStaticObject(ecs.get(), glm::vec3(0.0f, 5.0f, -15.0f), StaticObject::TREE);
        
        // Global Light Source
        // auto sun = addLightSource(ecs.get(), glm::vec3(-0.3f, -1.0f, -0.5f), LightType::Directional); // Disabled for 2D conversion
        
        // Create Player
        size_t player1 = ecs -> createEntity();
        ecs -> assignArchetype(player1, playerArchetype);
        ecs -> assignArchetype(player1, renderableArchetype);
        ecs -> assignArchetype(player1, physicsArchetype);
        // ecs -> assignArchetype(player1, animationArchetype); // Disabled for 2D conversion
        RenderableComponent playerRenderable;
        initializePlayerRenderable(playerRenderable);
        TransformComponent playerTransform;
        playerTransform.scale = glm::vec3(0.5f);
        playerTransform.position = glm::vec3(0.0f, 16.0f, 0.0f);
        playerTransform.rotation = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);


        PlayerComponent playerComponent;
        PhysicsComponent2D playerPhysics;
        SpriteComponent playerSprite;
        SkeletonComponent playerSkeleton;
        AnimationComponent playerAnimation;


        // playerRenderable.m_model = ResourceManager<Model>::getInstance().get("riggedFigure"); // Disabled for 2D conversion
        // playerRenderable.m_model -> printAnimations(); // Disabled for 2D conversion
        // playerAnimation.animationMap = playerRenderable.m_model -> populateAnimationMap(); // Disabled for 2D conversion
        playerAnimation.currentAnimation = "Idle_01";
        size_t animationIndex = playerAnimation.animationMap[playerAnimation.currentAnimation];
        // playerSkeleton.boneMatrices = playerRenderable.m_model -> calculateFinalBoneMatrices(0.0f, animationIndex); // Disabled for 2D conversion
        playerRenderable.shader = ResourceManager<Shader>::getInstance().get("modelShader");
        playerRenderable.textureID = ResourceManager<Texture>::getInstance().get("playerTexture") -> getID();
        ColliderComponent2D playerCollider;
        playerCollider.size = glm::vec3(0.3f, 3.0f, 0.3f);
        
        auto playerInventory = ecs -> createEntity();
        ecs->assignArchetype(playerInventory, inventoryArchetype);
        InventoryComponent playerInventoryComponent;
        playerInventoryComponent.items.push_back(GameItemComponent({"ammunition", 10}));
        playerInventoryComponent.items.push_back(GameItemComponent({"shovel", 1}));
        playerInventoryComponent.items.push_back(GameItemComponent({"handgun", 10}));
        ecs->addComponent(playerInventory, playerInventoryComponent);
        
        playerComponent.inventoryID = playerInventory;
        ecs -> addComponent(player1, playerRenderable);
        ecs -> addComponent(player1, playerCollider);
        ecs -> addComponent(player1, playerTransform);
        ecs -> addComponent(player1, playerComponent);
        ecs -> addComponent(player1, playerSprite);
        ecs -> addComponent(player1, playerPhysics);
        ecs -> addComponent(player1, playerSkeleton);
        ecs -> addComponent(player1, playerAnimation);
        
        
        // Inventory Bar
        size_t inventoryBar = ecs -> createEntity();
        ecs -> assignArchetype(inventoryBar, uiArchetype);
        UITextElement inventoryBarTextElement("", "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, false);
        UIImageElement inventoryBarImageElement("inventoryBarTexture", true);
        UITransform inventoryBarTransform(glm::vec3(400.0f, 750.0f, -0.1f), glm::vec2(800.0f, 100.0f), glm::vec2(1.0f, 1.0f));
        UIInput inventoryBarInput(
            [](){},
            [](){}
        );
        InventoryBarComponent inventoryBarComponent;
        updateInventoryBar(ecs.get(), inventoryBarComponent, playerInventoryComponent, itemRegistry);
        ecs -> addComponent(inventoryBar, inventoryBarTextElement);
        ecs -> addComponent(inventoryBar, inventoryBarImageElement);
        ecs -> addComponent(inventoryBar, inventoryBarTransform);
        ecs -> addComponent(inventoryBar, inventoryBarInput);
        ecs -> addComponent(inventoryBar, inventoryBarComponent);
        
        
        
        
        // Starting Chunk
        int viewDistance = chunkSystem -> viewDistance;
        int i = worldMap[0].size() / 2;
        int j = worldMap.size() / 2;
        for (int x = i - viewDistance; x <= viewDistance + i; ++x) {
        for (int y = j - viewDistance; y <= viewDistance + j; ++y) {
            
            glm::ivec3 chunkCoord = glm::ivec3(x - i, y - j,0);
            ChunkComponent chunk(chunkCoord, 16, 16, 40, 1.0f, worldMap[x][y].finalElevation);
            size_t chunkEntity = addChunk(ecs.get(), worldMap, chunk);
            chunkSystem ->chunkRegistry.insert({chunkCoord, chunkEntity});
        }
        }
        // Create Test NPC's
        size_t testNPC = addNPC(ecs.get(), glm::vec3(1.0f, 16.0f, 3.0f), "human", "Jim Flicky", "Human", 0, NPCState::Friendly);
        size_t testNPC1 = addNPC(ecs.get(), glm::vec3(0.0f, 16.0f, 0.0f), "human", "Zombie", "Undead", 1, NPCState::Hostile);
        size_t testNPC2 = addNPC(ecs.get(), glm::vec3(3.0f, 16.0f, 0.0f), "human", "GrubGrub", "Orc", 3, NPCState::Neutral);
        
        renderSystem -> cameraEntity = cameraEntity;
        chunkSystem -> cameraEntity = cameraEntity;
        chunkSystem -> player = player1;
        playerSystem -> cameraEntity = cameraEntity;
        chunkSystem -> map = worldMap;
        chunkSystem -> w_coord = worldCoordinates;
        chunkSystem -> c_coord = chunkCoordinates;
        chunkSystem -> t_coord = tileCoordinates;
        playerSystem -> textBox = textBox;
        cameraSystem -> playerEntity = player1;
        physicsSystem -> chunkSystem = chunkSystem.get();
        playerSystem -> chunkSystem = chunkSystem.get();
        playerSystem -> itemRegistry = &itemRegistry;
        playerSystem -> ecs = ecs.get();
        inventorySystem -> itemRegistry = &itemRegistry;
        inventorySystem -> inventoryBar = inventoryBar;

        inventorySystem -> setEventQueue(m_eventQueue);
        playerSystem -> setEventQueue(m_eventQueue);

    }


    void registerItems(){
        std::vector<ItemDefinition> items = {
            {
                "shovel", 
                "shovel",
                "shovelIcon",
                "",
                ItemType::HELD,
                false,
                1
            },
            {
                "handgun", 
                "handgun",
                "handgunIcon",
                "",
                ItemType::HELD,
                false,
                1
            },
            {
                "ammunition", 
                "ammunition",
                "ammunitionIcon",
                "",
                ItemType::HELD,
                false,
                1
            },
            {
                "dirt",
                "dirt",
                "dirtIcon",
                "",
                ItemType::RESOURCE,
                true,
                64
            },
        };
        for (auto item : items)
            itemRegistry.registerItem(item);
        std::cout << "Items Registered\n";
    }
};

class GameSetupState : public GameState {
public:
    explicit GameSetupState(EventQueue& eventQueue) : GameState(eventQueue) {}
    void onEnter() override {
        init();
    }
    void update(float deltaTime) override {
        
        ecs -> updateSystems(deltaTime);
    }
private:
    std::vector<PlayerSlot> playerSlots;
    void init() {
        std::cout << "Init GameSetup State . . ." << "\n";
        auto uiSystem = ecs -> registerSystem<UISystem>();
        auto mouseInputSystem = ecs -> registerSystem<CursorSystem>();
        auto inputSystem = ecs -> registerSystem<UIInputSystem>();
        auto playerSlotSystem = ecs -> registerSystem<PlayerSlotSelectionSystem>();
        auto mapSettingsSystem = ecs -> registerSystem<MapSettingsSelectionSystem>();


        ecs -> setSystemSignature<UISystem>(uiArchetype);
        ecs -> setSystemSignature<CursorSystem>(cursorArchetype);
        ecs -> setSystemSignature<UIInputSystem>(inputArchetype);

        size_t titleEntity = ecs -> createEntity();
        ecs -> assignArchetype(titleEntity, uiArchetype);
        UITextElement titleTextElement("Create Game", "titleFont", glm::vec3(0.0f, 0.0f, 0.0f), 50.0f, true);
        UIImageElement titleImageElement("buttonTexture", false);
        UITransform titleTransform(glm::vec3(150.0f, 700.0f, 0.0f), glm::vec2(120.0f, 40.0f), glm::vec2(1.0f, 1.0f));
        UIInput titleInput;
        ecs -> addComponent(titleEntity, titleTextElement);
        ecs -> addComponent(titleEntity, titleImageElement);
        ecs -> addComponent(titleEntity, titleTransform);
        ecs -> addComponent(titleEntity, titleInput);

        // Start Game Button
        size_t playersHeading = ecs -> createEntity();
        ecs -> assignArchetype(playersHeading, uiArchetype);
        ecs -> assignArchetype(playersHeading, inputArchetype);
        
        UITextElement playersHeadingTextElement("Players", "headerFont", glm::vec3(0.0f), 30.0f, true);
        UIImageElement playersHeadingImageElement("buttonTexture", false);
        UITransform playersHeadingTransform(glm::vec3(100.0f, 600.0f, 0.0f), glm::vec2(120.0f, 100.0f), glm::vec2(1.0f, 1.0f));
        
        ecs -> addComponent(playersHeading, playersHeadingTextElement);
        ecs -> addComponent(playersHeading, playersHeadingImageElement);
        ecs -> addComponent(playersHeading, playersHeadingTransform);

        // Exit Button
        size_t exitButtonEntity = ecs -> createEntity();
        ecs -> assignArchetype(exitButtonEntity, uiArchetype);

        UITextElement exitTextElement("Map Settings", "headerFont", glm::vec3(0.0f), 22.0f, true);
        UIImageElement exitImageElement("buttonTexture", false);
        UITransform exitTransform(glm::vec3(100.0f, 300.0f, 0.0f), glm::vec2(120.0f, 100.0f), glm::vec2(1.0f, 1.0f));

        ecs -> addComponent(exitButtonEntity, exitTextElement);
        ecs -> addComponent(exitButtonEntity, exitImageElement);
        ecs -> addComponent(exitButtonEntity, exitTransform);

        // Cursor Entity
        size_t cursorEntity = ecs -> createEntity();
        ecs -> assignArchetype(cursorEntity, cursorArchetype);
        ecs -> assignArchetype(cursorEntity, uiArchetype);
        UITextElement cursorText("", "Faculty-Glyphic", glm::vec3(0.0f, 0.0f, 0.0f), 16.0f, false);
        UIImageElement cursorElement("cursorTexture", true);
        UITransform cursorTransform(glm::vec3(300.0f, 600.0f, 0.1f), glm::vec2(25.0f, 25.0f), glm::vec2(1.0f, 1.0f));
        
        CursorComponent cursorComponent(300.0f, 600.0f, 0.0f, 0.0f, 0.50f);
        ecs -> addComponent(cursorEntity, cursorElement);
        ecs -> addComponent(cursorEntity, cursorTransform);
        ecs -> addComponent(cursorEntity, cursorComponent);
        ecs -> addComponent(cursorEntity, cursorText);
        

        size_t startButtonEntity = ecs -> createEntity();
        ecs -> assignArchetype(startButtonEntity, uiArchetype);
        ecs -> assignArchetype(startButtonEntity, inputArchetype);
        
        UITextElement startButtonTextElement("Start", "Faculty-Glyphic", glm::vec3(0.0f), 22.0f, true);
        UIImageElement startButtonImageElement("buttonTexture", true);
        UITransform startButtonTransform(glm::vec3(1100.0f, 100.0f, 0.0f), glm::vec2(120.0f, 100.0f), glm::vec2(1.0f, 1.0f));
        UIInput startButtonInput(
            [this, playerSlots = &this -> playerSlots](){
                eventQueue.push(std::make_unique<GameEvent>(GameEvent(GameEventType::ChangeState, std::make_unique<GamePlayState>(eventQueue))));
            },
            [startButtonEntity, ecs = this -> ecs.get()](){
                auto& text = ecs->getComponent<UITextElement>(startButtonEntity);
                text.color = glm::vec3(1.0f, 1.0f, 0.0f);
            },
            [startButtonEntity, ecs = this -> ecs.get()](){
                auto& text = ecs->getComponent<UITextElement>(startButtonEntity);
                text.color = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        );
        ecs -> addComponent(startButtonEntity, startButtonTextElement);
        ecs -> addComponent(startButtonEntity, startButtonImageElement);
        ecs -> addComponent(startButtonEntity, startButtonTransform);
        ecs -> addComponent(startButtonEntity, startButtonInput);

        createPlayerSlots();
        createMapSettingsPanel();
        

        std::cout << "GameSetupState Initialized . . ." << "\n";

    }

void createPlayerSlots() {
    float startX = 100.0f; // Starting position for the first player slot
    float startY = 500.0f; // Base Y position
    float spacing = 150.0f; // Horizontal spacing between slots

    for (int i = 0; i < 4; i++) {
        size_t slotEntity = ecs->createEntity();
        size_t labelEntity = ecs->createEntity(); // Create a separate entity for the label

        // Assign archetypes for UI components
        ecs->assignArchetype(slotEntity, { typeid(PlayerSlot).hash_code(), typeid(UITransform).hash_code(), typeid(UIInput).hash_code() });
        ecs->assignArchetype(slotEntity, { typeid(UIImageElement).hash_code(), typeid(UITransform).hash_code() });
        ecs->assignArchetype(slotEntity, { typeid(UIInput).hash_code(), typeid(UITransform).hash_code() });

        // Assign archetype for the label
        ecs->assignArchetype(labelEntity, { typeid(UIImageElement).hash_code(), typeid(UITransform).hash_code() });

        // Player slot properties
        float slotX = startX + (i * spacing); // Distribute slots horizontally
        PlayerSlot slot(i);
        UITransform slotTransform(glm::vec3(slotX, startY, 0.0f), glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f));
        UIImageElement slotImage("buttonTexture", true);
        UITextElement slotText("Empty", "Faculty-Glyphic", glm::vec3(0.0f), 22.0f, true);

        // Label for the slot
        UITransform labelTransform(glm::vec3(slotX, startY + 60.0f, 0.0f), glm::vec2(100.0f, 30.0f), glm::vec2(1.0f, 1.0f));
        UITextElement labelText("Player " + std::to_string(i + 1), "Faculty-Glyphic", glm::vec3(0.0f), 20.0f, true);
        UIImageElement labelImage("buttonTexture", false);

        // Capture ECS pointer and entity ID for the input lambda
UIInput input(
    [slotEntity, playerSlots = &this->playerSlots, ecs = this->ecs.get()]() { // onClick
        if (!ecs->hasComponent<PlayerSlot>(slotEntity) || !ecs->hasComponent<UITextElement>(slotEntity)) {
            return;
        }

        auto& slot = ecs->getComponent<PlayerSlot>(slotEntity);
        auto& text = ecs->getComponent<UITextElement>(slotEntity);

        // Find the corresponding slot in the vector
        auto it = std::find_if(playerSlots->begin(), playerSlots->end(), 
            [&](const PlayerSlot& p) { return p.slotIndex == slot.slotIndex; });

        // Toggle PlayerSlot state
        switch (slot.playerType) {
            case PlayerType::EMPTY:
                slot.playerType = PlayerType::HUMAN;
                text.text = "Human";
                text.color = glm::vec3(0.0f, 1.0f, 0.0f); // Green for human
                
                if (it == playerSlots->end()) {
                    playerSlots->push_back({slot.slotIndex, PlayerType::HUMAN});
                } else {
                    it->playerType = PlayerType::HUMAN;
                }
                break;

            case PlayerType::HUMAN:
                slot.playerType = PlayerType::AI;
                text.text = "AI";
                text.color = glm::vec3(1.0f, 0.0f, 0.0f); // Red for AI
                
                if (it != playerSlots->end()) {
                    it->playerType = PlayerType::AI;
                }
                break;

            case PlayerType::AI:
                slot.playerType = PlayerType::EMPTY;
                text.text = "Empty";
                text.color = glm::vec3(0.0f); // Default color
                
                if (it != playerSlots->end()) {
                    playerSlots->erase(it); // Remove the slot if it becomes EMPTY
                }
                break;
        }
    },

    [slotEntity, ecs = this->ecs.get()]() { // onHover
        if (ecs->hasComponent<UITextElement>(slotEntity)) {
            auto& text = ecs->getComponent<UITextElement>(slotEntity);
            text.color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue when hovered
            ecs->addComponent(slotEntity, text);
        }
    },
    [slotEntity, ecs = this->ecs.get()]() { // onHoverExit
        if (ecs->hasComponent<UITextElement>(slotEntity)) {
            auto& text = ecs->getComponent<UITextElement>(slotEntity);
            text.color = glm::vec3(0.0f); // Reset to original color
            ecs->addComponent(slotEntity, text);
        }
    }
);


        // Add components to ECS
        ecs->addComponent(slotEntity, slot);
        ecs->addComponent(slotEntity, slotTransform);
        ecs->addComponent(slotEntity, slotText);
        ecs->addComponent(slotEntity, input);
        ecs->addComponent(slotEntity, slotImage);

        // Add label to ECS
        ecs->addComponent(labelEntity, labelTransform);
        ecs->addComponent(labelEntity, labelText);
        ecs->addComponent(labelEntity, labelImage);
    }
}




    void createMapSettingsPanel() {
        size_t mapEntity = ecs->createEntity();
        ecs->assignArchetype(mapEntity, { typeid(MapSettings).hash_code(), typeid(UITransform).hash_code(), typeid(UIInput).hash_code() });

        MapSettings settings;
        UITransform transform(glm::vec3(300.0f, 500.0f, 0.0f), glm::vec2(200.0f, 50.0f), glm::vec2(1.0f, 1.0f));
        UIInput input;

        ecs->addComponent(mapEntity, settings);
        ecs->addComponent(mapEntity, transform);
        ecs->addComponent(mapEntity, input);
    }


};

class MainMenuState : public GameState {
public:
    explicit MainMenuState(EventQueue& eventQueue) : GameState(eventQueue) {}
    void onEnter() override {
        init();
    }
    void update(float deltaTime) override {
        
        ecs -> updateSystems(deltaTime);
    }

private:

    void init() {
        // Register UI System and Components
        std::cout << "Init Main Menu State . . ." << "\n";

        // setup ECS
        auto uiSystem = ecs -> registerSystem<UISystem>();
        auto mouseInputSystem = ecs -> registerSystem<CursorSystem>();
        auto inputSystem = ecs -> registerSystem<UIInputSystem>();


        ecs -> setSystemSignature<UISystem>(uiArchetype);
        ecs -> setSystemSignature<CursorSystem>(cursorArchetype);
        ecs -> setSystemSignature<UIInputSystem>(inputArchetype);


        // title entity
        size_t titleEntity = ecs -> createEntity();
        ecs -> assignArchetype(titleEntity, uiArchetype);
        UITextElement titleTextElement("HexRTS", "titleFont", glm::vec3(0.0f, 0.0f, 0.0f), 50.0f, true);
        UIImageElement titleImageElement("buttonTexture", false);
        UITransform titleTransform(glm::vec3(100.0f, 700.0f, 0.0f), glm::vec2(120.0f, 40.0f), glm::vec2(1.0f, 1.0f));
        UIInput titleInput;
        ecs -> addComponent(titleEntity, titleTextElement);
        ecs -> addComponent(titleEntity, titleImageElement);
        ecs -> addComponent(titleEntity, titleTransform);
        ecs -> addComponent(titleEntity, titleInput);

        // Start Game Button
        size_t startButtonEntity = ecs -> createEntity();
        ecs -> assignArchetype(startButtonEntity, uiArchetype);
        ecs -> assignArchetype(startButtonEntity, inputArchetype);
        UITextElement startTextElement("New Game", "Faculty-Glyphic", glm::vec3(0.0f), 30.0f, true);
        UIImageElement startImageElement("buttonTexture", true);
        UITransform startTransform(glm::vec3(600.0f, 400.0f, 0.0f), glm::vec2(120.0f, 120.0f), glm::vec2(1.0f, 1.0f));
        UIInput startInput(
            [this]() {
                eventQueue.push(std::make_unique<GameEvent>(GameEvent(GameEventType::ChangeState, std::make_unique<GamePlayState>(eventQueue))));
            },
            [](){}
        );
        ecs -> addComponent(startButtonEntity, startTextElement);
        ecs -> addComponent(startButtonEntity, startImageElement);
        ecs -> addComponent(startButtonEntity, startTransform);
        ecs -> addComponent(startButtonEntity, startInput);

        // Exit Button
        size_t exitButtonEntity = ecs -> createEntity();
        ecs -> assignArchetype(exitButtonEntity, uiArchetype);
        ecs -> assignArchetype(exitButtonEntity, inputArchetype);
        UITextElement exitTextElement("Exit", "Faculty-Glyphic", glm::vec3(0.0f), 22.0f, true);
        UIImageElement exitImageElement("buttonTexture", true);
        UITransform exitTransform(glm::vec3(600.0f, 200.0f, 0.0f), glm::vec2(120.0f, 120.0f), glm::vec2(1.0f, 1.0f));
        UIInput exitInput(
            [this]() {
                eventQueue.push(std::make_unique<GameEvent>(GameEvent(GameEventType::ExitApplication)));
            },
            [this](){

            }
        );;
        ecs -> addComponent(exitButtonEntity, exitTextElement);
        ecs -> addComponent(exitButtonEntity, exitImageElement);
        ecs -> addComponent(exitButtonEntity, exitTransform);
        ecs -> addComponent(exitButtonEntity, exitInput);


        // Cursor Entity
        size_t cursorEntity = ecs -> createEntity();
        ecs -> assignArchetype(cursorEntity, cursorArchetype);
        ecs -> assignArchetype(cursorEntity, uiArchetype);
        UITextElement cursorText("", "Faculty-Glyphic", glm::vec3(0.0f, 0.0f, 0.0f), 16.0f, false);
        UIImageElement cursorElement("cursorTexture", true);
        UITransform cursorTransform(glm::vec3(300.0f, 600.0f, 0.1f), glm::vec2(25.0f, 25.0f), glm::vec2(1.0f, 1.0f));
        
        CursorComponent cursorComponent(300.0f, 600.0f, 0.0f, 0.0f, 0.50f);
        ecs -> addComponent(cursorEntity, cursorElement);
        ecs -> addComponent(cursorEntity, cursorTransform);
        ecs -> addComponent(cursorEntity, cursorComponent);
        ecs -> addComponent(cursorEntity, cursorText);
        
        

        std::cout << "Main Menu State Initialized . . ." << "\n";
    }

};


