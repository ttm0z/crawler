#pragma once
#include "glad/glad.h"
#include <unordered_map>
#include <deque>
#include <queue>
#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include "../Shader.hpp"
#include "../CommandTypes.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


enum class PlayerType {EMPTY, HUMAN, AI};
enum class StaticObject {TREE, ROCK};

enum class LightType {
    Directional,
    Point,
    Spot
};

struct LightSourceComponent2D {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    float radius = 5.0f;
    bool enabled = true;
};

struct SkyboxComponent {
    std::vector<GLfloat> skyboxVertices = {
    // positions
    -1.0f,  1.0f, -1.0f, // Front
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, // Back
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f, // Right
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, // Left
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f, // Top
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f, // Bottom
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
    std::string skyboxShader;
    std::string cubemapId;
};


struct PlayerSlot {
    PlayerType playerType;
    glm::vec3 color; 
    int slotIndex;
    bool selected;

    // Constructor with default values
    PlayerSlot(int index = -1, 
               PlayerType type = PlayerType::EMPTY, 
               glm::vec3 col = glm::vec3(1.0f), 
               bool sel = false) 
        : slotIndex(index), playerType(type), color(col), selected(sel) {}
};


struct MapSettings {
    glm::ivec2 mapSize;
    std::string mapShape;
    std::string preset;

    MapSettings() : mapSize(glm::ivec2(128, 128)), mapShape("Hex"), preset("Default") {}
};

struct UISelectable {
    bool isSelected;
    std::function<void()> onClick; // Callback function for actions

    UISelectable(std::function<void()> clickAction) : isSelected(false), onClick(clickAction) {}
};

struct PlayerCommand{
    CommandType type;
    int playerID;
    PlayerCommand(int ID, CommandType type) 
        : playerID(ID), type(type) {}
};

struct PlayerComponent {
    int playerID;
    bool isWalking = false;
    glm::vec3 position;
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right;
    std::vector<PlayerCommand> commandQueue;
    float movementSpeed = 2.5f;
    float jumpHeight = 0.5f;
    size_t inventoryID;
};

enum class NPCState{
    Friendly,
    Neutral,
    Hostile
};

struct NPCComponent {
    static inline int nextID = 0;

    int id;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;

    std::string name;
    std::string race;
    
    int level;
    NPCState attitude;

    glm::ivec2 currentChunk;
    glm::ivec2 destinationChunk;
    std::queue<glm::ivec3> path;

    bool hasPath;
    bool reachedDestinationChunk;

    float movementSpeed;
    float jumpHeight;

    NPCComponent()
        : id(nextID++),
          position(0.0f),
          direction(0.0f, 0.0f, 1.0f),
          right(1.0f, 0.0f, 0.0f),
          currentChunk(0, 0),
          destinationChunk(0, 0),
          hasPath(false),
          reachedDestinationChunk(false),
          movementSpeed(2.5f),
          jumpHeight(0.5f)
    {}
};


struct AIPlayerComponent {
    int difficulty;
};

struct PopulationUnitComponent {
    glm::vec2 position;
    glm::vec2 target;
    size_t ownerID;
    glm::vec2 worldPosition;
    glm::vec2 worldTarget;
    glm::vec3 color;
    std::deque<glm::ivec2> path;
    bool isAttacking = false;
    bool isMoving = false;
    float movementProgress = 0.0f;
};

struct HexTile {
    glm::vec3 position;
    int texIndex = 0;
    HexTile(int tileX = 0, int tileY = 0, int tileZ = 0)
        : position{tileX, tileY, tileZ}{}
};

enum class TileType {
    GRASS,
    SAND,
    ROCK,
    DIRT,
    WATER,
    EMPTY
};

enum class BiomeType {
    Ocean,
    Beach,
    Grassland,
    Forest,
    Rainforest,
    Desert,
    Tundra,
    Taiga,
    Swamp,
    Mountain,
    Snow
};

struct ChunkData {
    float baseElevation = 0.0f;
    float finalElevation = 0.0f;
    float moisture = 0.0f;
    float temperature = 0.0f;
    BiomeType biome = BiomeType::Ocean;
    float riverFlow = 0.0f;
    float fertility = 0.0f;
    float vegetationDensity = 0.0f;
};

struct ChunkComponent {
    std::vector<HexTile> tiles;  // Contiguous storage for all hex tiles in the chunk
    std::vector<TileType> palette = {TileType::GRASS, TileType::EMPTY};
    std::vector<uint8_t> occupancy;
    std::vector<uint8_t> tileIndices;
    
    glm::ivec3 chunkIndex;       // Chunk position in world (using 3D index)
    int rows;
    int columns;
    int depth;
    int elevation;
    float hexRadius;
    bool dirty;
    bool toDraw;
    
    ChunkComponent(glm::ivec3 index = glm::ivec3(0, 0, 0),
                   int rows = 1, int columns = 1, int depth = 1,
                   float radius = 1.0f, int elevation = 0)
        : chunkIndex(index), rows(rows), columns(columns), depth(depth),
          hexRadius(radius), elevation(elevation), dirty(false)
    {
        tiles.resize(rows * columns * depth);
        occupancy.resize(rows * columns * depth);
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < columns; ++y) {
                for (int x = 0; x < rows; ++x) {
                    int idx = indexFrom3D(x, y, z);
                    tiles[idx] = HexTile(x, y, z);
                    occupancy[idx] = 0;
                }
            }
        }
    }
    
    inline int indexFrom3D(int x, int y, int z) const {
        return x + rows * (y + columns * z);
    }
    
    // Accessor: get a tile reference at (x, y, z)
    HexTile& getTile(int x, int y, int z) {
        return tiles[indexFrom3D(x, y, z)];
    }
    
    const HexTile& getTile(int x, int y, int z) const {
        return tiles[indexFrom3D(x, y, z)];
    }
    
    // Mutator: update the tile at (x, y, z) and mark the chunk as dirty.
    void setTile(int x, int y, int z, const HexTile& tile) {
        tiles[indexFrom3D(x, y, z)] = tile;
        dirty = true;
    }
};


struct TextBoxComponent {
    std::string text;
    int width;
    int height;
};
struct SpriteComponent {
    std::shared_ptr<Shader> shader;
    GLuint textureID;
    glm::vec2 textureOffset = glm::vec2(0.0f);
    glm::vec2 textureSize = glm::vec2(1.0f);
    glm::vec4 color = glm::vec4(1.0f);
    bool flipX = false;
    bool flipY = false;
    
    // Atlas support
    std::string spriteName = "";     // Name of sprite in atlas (empty for direct texture)
    std::string atlasName = "";      // Name of atlas containing sprite
    int renderLayer = 0;             // Rendering layer for depth sorting
    bool useBatching = true;         // Whether to use batched rendering

    SpriteComponent(
        GLuint texture = 0,
        std::shared_ptr<Shader> shaderProgram = nullptr,
        glm::vec4 spriteColor = glm::vec4(1.0f))
        : shader(shaderProgram), textureID(texture), color(spriteColor) {}
        
    // Constructor for atlas-based sprites
    SpriteComponent(
        const std::string& sprite, 
        const std::string& atlas,
        glm::vec4 spriteColor = glm::vec4(1.0f),
        int layer = 0)
        : shader(nullptr), textureID(0), color(spriteColor), 
          spriteName(sprite), atlasName(atlas), renderLayer(layer) {}
};

struct TileComponent {
    int tileType = 0;  // Index into tileset
    bool isWalkable = true;
    bool isVisible = true;
    GLuint textureID = 0;
    glm::vec2 textureCoords = glm::vec2(0.0f); // UV coordinates in tileset
};

struct DungeonRoomComponent {
    std::vector<TileComponent> tiles;
    glm::ivec2 roomSize;
    glm::ivec2 roomPosition;
    bool isGenerated = false;
    
    DungeonRoomComponent(glm::ivec2 size = glm::ivec2(10, 10), 
                         glm::ivec2 position = glm::ivec2(0, 0))
        : roomSize(size), roomPosition(position) {
        tiles.resize(size.x * size.y);
    }
    
    inline int getTileIndex(int x, int y) const {
        return x + y * roomSize.x;
    }
    
    TileComponent& getTile(int x, int y) {
        return tiles[getTileIndex(x, y)];
    }
    
    const TileComponent& getTile(int x, int y) const {
        return tiles[getTileIndex(x, y)];
    }
};

struct StaticObjectComponent {
    std::vector<glm::ivec3> occupiedTiles;
};
struct SelectionBoxComponent {
    glm::vec2 startPos;
    glm::vec2 endPos;
    bool visible = false;
    std::vector<GLfloat> vertices = {
        1.0f, 1.0f, -0.1f,  // Bottom-left
        1.0f,   1.0f, -0.1f,  // Bottom-right
        1.0f,   1.0f,   -0.1f,  // Top-right
        1.0f, 1.0f,   -0.1f   // Top-left
    };

    std::vector<GLuint> indices = {
        0, 1, 2, // First triangle
        2, 0,3  // Second triangle
    };
};
struct PhysicsComponent2D {
    glm::vec2 position;
    glm::vec2 velocity = glm::vec2(0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f);
    float rotation = 0.0f;
    float angularVelocity = 0.0f;
    
    float mass = 1.0f;
    float inverseMass = 1.0f;
    
    glm::vec2 force = glm::vec2(0.0f);
    float torque = 0.0f;
    
    float linearDamping = 0.9f;
    float angularDamping = 0.9f;
    
    bool isStatic = false;
};

struct ColliderComponent2D {
    glm::vec2 size = glm::vec2(1.0f, 1.0f);
    glm::vec2 offset = glm::vec2(0.0f);
    bool isTrigger = false;
    bool isStatic = false;
};
enum class ItemType {
    NONE,
    HELD,
    ARMOR,
    CONSUMABLE,
    RESOURCE,
    KEY_ITEM
};

struct GameItemComponent {
    std::string itemID;
    int quantity = 1;
    ItemType itemType = ItemType::NONE;
};
struct InventoryComponent {
    std::vector<GameItemComponent> items;
    std::string selected;
    int maxSlots = 10;
};


struct InventoryBarComponent {
    std::vector<std::string> itemSlots; // item IDs
    std::vector<std::string> itemNames; 
    int selectedSlot = 0;
};

struct WeaponComponent {
    int damage = 0;
    float attackSpeed = 1.0f;
    bool isRanged = false;

    WeaponComponent(int damage = 0, float attackSpeed = 1.0f, bool isRanged = false)
        : damage(damage), attackSpeed(attackSpeed), isRanged(isRanged) {}
};
struct ArmorComponent {
    int armorRating = 0;
    float durability = 100.0f;

    ArmorComponent(int armorRating = 0, float durability = 100.0f)
        : armorRating(armorRating), durability(durability) {}
};
struct ConsumableComponent {
    int healAmount = 0;
    float duration = 0.0f;

    ConsumableComponent(int healAmount = 0, float duration = 0.0f)
        : healAmount(healAmount), duration(duration) {}
};

struct CameraComponent3D {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    float fov;
    float yaw;
    float pitch;
    float nearPlane;
    float farPlane;
    float aspectRatio;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    // Constructor
    CameraComponent3D(
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& direction = glm::vec3(0.0f, 0.0f, -1.0f),
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
        float fov = 45.0f,
        float yaw = -90.0f,
        float pitch = 0.0f,
        float nearPlane = 0.1f,
        float farPlane = 100.0f,
        float aspectRatio = 16.0f / 9.0f
    ) : position(position),
        yaw(yaw), pitch(pitch),
        direction(glm::normalize(direction)),
        up(glm::normalize(up)),
        right(glm::normalize(glm::cross(direction, up))),
        fov(fov),
        nearPlane(nearPlane),
        farPlane(farPlane),
        aspectRatio(aspectRatio) {
        updateVectors();
        updateMatrices();
    }

    // Update projection and view matrices
    void updateMatrices() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        viewMatrix = glm::lookAt(position, position + direction, up);
    }
    void updateVectors() {
        // Convert yaw & pitch to a normalized direction vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(front);

        // Recalculate right and up vectors
        right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))); 
        up = glm::normalize(glm::cross(right, direction));
    }
};

struct CameraComponent2D {
    glm::vec2 position;  // Camera position in 2D space
    float rotation;      // Rotation of the camera in degrees (optional for 2D)
    float zoom;          // Zoom level
    float nearPlane;     // Near clipping plane
    float farPlane;      // Far clipping plane
    float aspectRatio;   // Aspect ratio (width / height)
    float panX;          // Horizontal panning offset
    float panY;          // Vertical panning offset
    glm::mat4 projectionMatrix; // Orthographic projection matrix
    glm::mat4 viewMatrix;       // View matrix

    // Constructor
    CameraComponent2D(
        const glm::vec2& position = glm::vec2(0.0f, 0.0f),
        float rotation = 0.0f,
        float zoom = 1.0f,
        float nearPlane = -1.0f,
        float farPlane = 1.0f,
        float aspectRatio = 16.0f / 9.0f,
        float panOffsetX = 0.0f,
        float panOffsetY = 0.0f
    ) : position(position),
        rotation(rotation),
        zoom(zoom),
        nearPlane(nearPlane),
        farPlane(farPlane),
        aspectRatio(aspectRatio),
        panX(panOffsetX),
        panY(panOffsetY) {
        updateMatrices();
    }

    // Update projection and view matrices
    void updateMatrices() {
        // Orthographic projection matrix: incorporate zoom
        float orthoLeft = -aspectRatio / (zoom > 0 ? zoom : 1.0f);
        float orthoRight = aspectRatio / (zoom > 0 ? zoom : 1.0f);
        float orthoBottom = -1.0f / (zoom > 0 ? zoom : 1.0f);
        float orthoTop = 1.0f / (zoom > 0 ? zoom : 1.0f);

        projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, nearPlane, farPlane);

        // View matrix (applies translation and rotation)
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
        glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        // Combine rotation and translation into the view matrix
        viewMatrix = rotationMat * translation;
        
        // Apply the pan to the view matrix
        glm::mat4 panMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(panX, panY, 0.0f));
        viewMatrix = panMatrix * viewMatrix; // Apply pan to view matrix
    }
};


struct CursorComponent {
    float mouseX;
    float mouseY;
    float xOffset;
    float yOffset;
    float sensitivity;
    bool is3D;

    CursorComponent(float mouseX = 0.0f, float mouseY = 0.0f, 
        float xOffset = 0.0f, float yOffset = 0.0f, float sensitivity = 0.05f, bool is3D = false) {}
};


struct TimerComponent {
    double elapsedTime = 0.0;
    double interval = 2.5f;
};

struct InputComponent {
    bool keys[1024];
    bool mouseButtons[3];
    glm::vec2 mousePosition;
    glm::vec2 mouseDelta;
    float scrollDelta;

    // Constructor
    InputComponent()
        : keys{}, mouseButtons{}, mousePosition(0.0f), mouseDelta(0.0f), scrollDelta(0.0f) {}
};

struct RenderableComponent {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint vertexCount;
    GLuint textureID;
    std::shared_ptr<Shader> shader;

    RenderableComponent(
        GLuint vao = 0,
        GLuint vbo = 0,
        GLuint ebo = 0,        
        GLuint vertexCount = 0,
        GLuint texture = 0,
        std::shared_ptr<Shader> shaderProgram = nullptr)
        : VAO(vao), VBO(vbo), EBO(ebo), vertexCount(vertexCount), textureID(texture), shader(shaderProgram) {}
};
struct AnimationComponent {
    std::string currentAnimation;
    float currentTime = 0.0f;
    bool isPlaying = true;
    bool loop = true;
    bool isPose = false;

    std::unordered_map<std::string, size_t> animationMap;
};
struct SkeletonComponent {
    std::vector<glm::mat4> boneMatrices;
};
struct TransformComponent {
    glm::vec3 position;
    glm::vec3 rotation;  // Euler angles
    glm::vec3 scale;

    TransformComponent(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 rot = glm::vec3(0.0f),
        glm::vec3 scl = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {}
};

struct TransformComponent2D {
    glm::vec3 position;
    glm::vec2 rotation;
    glm::vec2 scale;

    TransformComponent2D(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec2 rot = glm::vec3(0.0f),
        glm::vec2 scl = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {}
};

struct ShaderDataComponent {
    std::string shaderName; // Name of the shader
    std::unordered_map<std::string, glm::mat4> matrices;
    std::unordered_map<std::string, int> textures;      
    std::unordered_map<std::string, float> scalars;     
    std::unordered_map<std::string, glm::vec3> vectors; 

    ShaderDataComponent(const std::string& shader) : shaderName(shader) {}
};


// Position Component
struct Position {
    float x, y, z;

    // Constructor
    Position(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : x(x), y(y), z(z) {}
};

struct NPCInteractionElements {
    size_t panel;
    size_t headerText;
    size_t levelText;
    size_t npcPortrait;
    size_t npcDialogue;
    size_t playerDialogue;
};

struct InventoryMenuElements {
    size_t panel;
    size_t headerText;
    std::vector<size_t> inventoryList;
    std::vector<size_t> craftingList;
    size_t selectedItemDescription;
    size_t craftButton;
};


struct GameStateComponent {
    bool isGameplayFrozen = false;
};

// UI Element Component
struct UITextElement {
    std::string text;
    std::string fontCode;
    glm::vec3 color;
    float fontsize;
    bool isTextVisible;
    

    // Constructor
    UITextElement(
            const std::string text = "",
            const std::string fontCode = "",
            const glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
            float fontsize = 12.0f,
            bool isTextVisible = true)
        : text(text), fontCode(fontCode), color(color), fontsize(fontsize),
          isTextVisible(isTextVisible) {}
};

struct UIImageElement {
    std::string textureCode;
    bool isImageVisible;

    // Constructor
    UIImageElement(const std::string textureCode = "", bool isImageVisible = false)
        : textureCode(textureCode), isImageVisible(isImageVisible) {}
};
// UI Input Component
struct UIInput {
    std::function<void()> onClick;
    std::function<void()> onHover;
    std::function<void()> onHoverExit; // New callback for when hover ends
    bool wasClicked = false;  // Track previous click state
    bool wasHovered = false;  // Track previous hover state
    bool isSelected = false;

    // Constructor
    UIInput(const std::function<void()>& onClick = nullptr,
            const std::function<void()>& onHover = nullptr,
            const std::function<void()>& onHoverExit = nullptr)
        : onClick(onClick), onHover(onHover), onHoverExit(onHoverExit) {}

    void handleInput(bool isClicked, bool isHovered) {
        // Hover logic
        if (isHovered) {
            if (!wasHovered && onHover) {  // First frame of hovering
                onHover();
            }
        } else if (wasHovered) {  // Stopped hovering
            if (onHoverExit) {
                onHoverExit();
            }
        }

        // Detect first click transition (pressed this frame, but not last frame)
        if (isClicked && !wasClicked) {
            if (onClick) {
                onClick();
            }
        }

        // Update state for next frame
        wasClicked = isClicked;
        wasHovered = isHovered;
    }
};



// UI Transform Component
struct UITransform {
    glm::vec3 position;
    glm::vec2 size;
    glm::vec2 scale;

    // Constructor
    UITransform(const glm::vec3& position = glm::vec3(200.0f, 200.0f, 0.0f),
                const glm::vec2& size = glm::vec2(100.0f, 50.0f),
                const glm::vec2& scale = glm::vec2(1.0f, 1.0f))
        : position(position), size(size), scale(scale) {}
};
// Velocity Component
struct VelocityComponent {
    float vx, vy, vz;

    // Constructor
    VelocityComponent(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f)
        : vx(vx), vy(vy), vz(vz) {}
};








