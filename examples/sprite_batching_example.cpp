/*
 * Sprite Batching System Integration Example
 * 
 * This example demonstrates how to integrate the high-performance sprite batching
 * system into your existing ECS-based game. It shows:
 * 
 * 1. Setting up texture atlases
 * 2. Creating sprite entities with batching support
 * 3. Performance benchmarking
 * 4. Migration from legacy rendering
 */

#include "ECS/ECS.hpp"
#include "ECS/systems/OptimizedRenderSystem2D.hpp"
#include "TextureAtlas.hpp"
#include "SpriteBatcher.hpp"
#include "RenderBenchmark.hpp"
#include <iostream>

class SpriteRenderingExample {
public:
    void setupExample() {
        std::cout << "=== Sprite Batching System Example ===" << std::endl;
        
        // Initialize ECS
        initializeECS();
        
        // Set up texture atlases
        setupTextureAtlases();
        
        // Create example sprites
        createExampleSprites();
        
        // Run performance comparison
        runPerformanceComparison();
        
        std::cout << "Example complete!" << std::endl;
    }

private:
    ECS ecs;
    std::shared_ptr<OptimizedRenderSystem2D> renderSystem;
    
    void initializeECS() {
        std::cout << "\n1. Initializing ECS and Render System..." << std::endl;
        
        // Create optimized render system
        renderSystem = ecs.registerSystem<OptimizedRenderSystem2D>();
        
        // Set system signature for sprites
        ecs.setSystemSignature<OptimizedRenderSystem2D>({
            typeid(SpriteComponent).hash_code(),
            typeid(TransformComponent2D).hash_code()
        });
        
        // Initialize the render system
        renderSystem->init();
        
        // Create camera entity
        size_t cameraEntity = ecs.createEntity();
        ecs.addComponent(cameraEntity, CameraComponent2D(
            glm::vec2(0.0f, 0.0f),  // position
            0.0f,                    // rotation
            1.0f,                    // zoom
            -1.0f, 1.0f,            // near/far planes
            16.0f/9.0f              // aspect ratio
        ));
        
        renderSystem->cameraEntity = cameraEntity;
        
        std::cout << "   ECS initialized successfully" << std::endl;
    }
    
    void setupTextureAtlases() {
        std::cout << "\n2. Setting up Texture Atlases..." << std::endl;
        
        auto& atlasManager = TextureAtlasManager::getInstance();
        
        // Create different atlases for different sprite categories
        auto gameAtlas = atlasManager.createAtlas("game_sprites", 1024, 1024);
        auto uiAtlas = atlasManager.createAtlas("ui_sprites", 512, 512);
        auto effectsAtlas = atlasManager.createAtlas("effects", 1024, 1024);
        
        // Load sprites into atlases
        // Note: Replace these paths with your actual sprite files
        std::vector<std::pair<std::string, std::string>> gameSprites = {
            {"player", "./assets/textures/t1.png"},
            {"cursor", "./assets/textures/cursor_01.png"},
            {"shovel", "./assets/textures/shovel_01.png"},
            {"handgun", "./assets/textures/handgun_01.png"}
        };
        
        std::vector<std::pair<std::string, std::string>> uiSprites = {
            {"button", "./assets/textures/buttonTexture.png"},
            {"inventory_bar", "./assets/textures/inventoryBarTexture.png"},
            {"textbox", "./assets/textures/textbox_01.png"}
        };
        
        // Load game sprites
        for (const auto& sprite : gameSprites) {
            if (renderSystem->loadSpriteToAtlas(sprite.first, sprite.second, "game_sprites")) {
                std::cout << "   Loaded " << sprite.first << " into game atlas" << std::endl;
            } else {
                std::cout << "   Warning: Failed to load " << sprite.first << std::endl;
            }
        }
        
        // Load UI sprites  
        for (const auto& sprite : uiSprites) {
            if (renderSystem->loadSpriteToAtlas(sprite.first, sprite.second, "ui_sprites")) {
                std::cout << "   Loaded " << sprite.first << " into UI atlas" << std::endl;
            } else {
                std::cout << "   Warning: Failed to load " << sprite.first << std::endl;
            }
        }
        
        // Generate all atlases
        renderSystem->generateAtlases();
        
        std::cout << "   Texture atlases generated successfully" << std::endl;
    }
    
    void createExampleSprites() {
        std::cout << "\n3. Creating Example Sprites..." << std::endl;
        
        // Create a grid of sprites to demonstrate batching
        int gridSize = 10;
        float spacing = 2.0f;
        
        for (int x = 0; x < gridSize; ++x) {
            for (int y = 0; y < gridSize; ++y) {
                glm::vec3 position(
                    (x - gridSize/2) * spacing,
                    (y - gridSize/2) * spacing,
                    0.0f
                );
                
                // Vary sprite types and colors
                std::string spriteName = (x + y) % 2 == 0 ? "player" : "cursor";
                std::string atlasName = "game_sprites";
                
                glm::vec4 color(
                    0.5f + (x / float(gridSize)) * 0.5f,
                    0.5f + (y / float(gridSize)) * 0.5f,
                    0.8f,
                    1.0f
                );
                
                int layer = (x + y) % 3; // Distribute across 3 layers
                
                size_t entity = renderSystem->createSpriteEntity(
                    ecs, spriteName, atlasName, position, 
                    glm::vec2(1.0f), color, layer
                );
                
                // Add some sprites that move for animation testing
                if ((x + y) % 5 == 0) {
                    // These sprites could have movement components added later
                }
            }
        }
        
        std::cout << "   Created " << (gridSize * gridSize) << " sprite entities" << std::endl;
        
        // Create some UI sprites
        createUIExamples();
    }
    
    void createUIExamples() {
        std::cout << "   Creating UI sprite examples..." << std::endl;
        
        // Create UI elements at fixed screen positions
        std::vector<std::tuple<std::string, glm::vec3, glm::vec2>> uiElements = {
            {"button", glm::vec3(-8.0f, 6.0f, 0.0f), glm::vec2(2.0f, 1.0f)},
            {"button", glm::vec3(-8.0f, 4.0f, 0.0f), glm::vec2(2.0f, 1.0f)},
            {"inventory_bar", glm::vec3(0.0f, -7.0f, 0.0f), glm::vec2(8.0f, 1.5f)},
            {"textbox", glm::vec3(6.0f, 6.0f, 0.0f), glm::vec2(3.0f, 2.0f)}
        };
        
        for (const auto& element : uiElements) {
            renderSystem->createSpriteEntity(
                ecs, 
                std::get<0>(element),  // sprite name
                "ui_sprites",          // atlas name
                std::get<1>(element),  // position
                std::get<2>(element),  // scale
                glm::vec4(1.0f),       // color
                10                     // high layer for UI
            );
        }
        
        std::cout << "   Created " << uiElements.size() << " UI elements" << std::endl;
    }
    
    void runPerformanceComparison() {
        std::cout << "\n4. Running Performance Comparison..." << std::endl;
        
        RenderBenchmark benchmark;
        
        // Quick performance test
        BenchmarkConfig config;
        config.numSprites = 500;
        config.testDurationSeconds = 3.0f;
        config.enableBatching = true;
        config.enableCulling = true;
        
        std::cout << "   Running benchmark with batching enabled..." << std::endl;
        BenchmarkResults batchedResults = benchmark.runBenchmark(ecs, *renderSystem, config);
        
        config.enableBatching = false;
        std::cout << "   Running benchmark with batching disabled..." << std::endl;
        BenchmarkResults unbatchedResults = benchmark.runBenchmark(ecs, *renderSystem, config);
        
        // Compare results
        std::cout << "\n   === PERFORMANCE COMPARISON ===" << std::endl;
        std::cout << "   Batched Rendering:" << std::endl;
        std::cout << "     FPS: " << batchedResults.averageFPS << std::endl;
        std::cout << "     Draw Calls: " << batchedResults.averageDrawCalls << std::endl;
        std::cout << "     Batches: " << batchedResults.averageBatches << std::endl;
        
        std::cout << "\n   Unbatched Rendering:" << std::endl;
        std::cout << "     FPS: " << unbatchedResults.averageFPS << std::endl;
        std::cout << "     Draw Calls: " << unbatchedResults.averageDrawCalls << std::endl;
        std::cout << "     Batches: " << unbatchedResults.averageBatches << std::endl;
        
        double fpsImprovement = (batchedResults.averageFPS - unbatchedResults.averageFPS) / unbatchedResults.averageFPS * 100.0;
        double drawCallReduction = (1.0 - double(batchedResults.averageDrawCalls) / unbatchedResults.averageDrawCalls) * 100.0;
        
        std::cout << "\n   Performance Improvements:" << std::endl;
        std::cout << "     FPS Improvement: " << fpsImprovement << "%" << std::endl;
        std::cout << "     Draw Call Reduction: " << drawCallReduction << "%" << std::endl;
        std::cout << "   ===============================" << std::endl;
    }
};

// Migration Guide Functions
namespace MigrationGuide {
    
    void showLegacyVsNewComparison() {
        std::cout << "\n=== MIGRATION GUIDE: Legacy vs New System ===" << std::endl;
        
        std::cout << "\nLEGACY SPRITE CREATION:" << std::endl;
        std::cout << "   size_t entity = ecs.createEntity();" << std::endl;
        std::cout << "   ecs.addComponent(entity, TransformComponent2D(position, rotation, scale));" << std::endl;
        std::cout << "   ecs.addComponent(entity, SpriteComponent(textureID, shader, color));" << std::endl;
        
        std::cout << "\nNEW ATLAS-BASED SPRITE CREATION:" << std::endl;
        std::cout << "   // Method 1: Using render system helper" << std::endl;
        std::cout << "   size_t entity = renderSystem->createSpriteEntity(" << std::endl;
        std::cout << "       ecs, \"spriteName\", \"atlasName\", position, scale, color, layer);" << std::endl;
        
        std::cout << "\n   // Method 2: Manual creation with atlas support" << std::endl;
        std::cout << "   size_t entity = ecs.createEntity();" << std::endl;
        std::cout << "   ecs.addComponent(entity, TransformComponent2D(position, rotation, scale));" << std::endl;
        std::cout << "   ecs.addComponent(entity, SpriteComponent(\"spriteName\", \"atlasName\", color, layer));" << std::endl;
        
        std::cout << "\nMIGRATION STEPS:" << std::endl;
        std::cout << "   1. Load your textures into atlases during initialization" << std::endl;
        std::cout << "   2. Replace RenderSystem2D with OptimizedRenderSystem2D" << std::endl;
        std::cout << "   3. Update sprite creation code to use atlas names" << std::endl;
        std::cout << "   4. Set batching preferences per sprite if needed" << std::endl;
        std::cout << "   5. Enable performance monitoring to verify improvements" << std::endl;
        
        std::cout << "\nPERFORMANCE TUNING:" << std::endl;
        std::cout << "   // Toggle batching system-wide" << std::endl;
        std::cout << "   renderSystem->enableBatching = true;" << std::endl;
        std::cout << "   " << std::endl;
        std::cout << "   // Toggle frustum culling" << std::endl;
        std::cout << "   renderSystem->enableFrustumCulling = true;" << std::endl;
        std::cout << "   " << std::endl;
        std::cout << "   // Show debug performance info" << std::endl;
        std::cout << "   renderSystem->showDebugInfo = true;" << std::endl;
        
        std::cout << "===============================================" << std::endl;
    }
    
    void showAtlasSetupExample() {
        std::cout << "\n=== ATLAS SETUP EXAMPLE ===" << std::endl;
        
        std::cout << "// 1. Create atlases for different sprite categories" << std::endl;
        std::cout << "auto& atlasManager = TextureAtlasManager::getInstance();" << std::endl;
        std::cout << "auto gameAtlas = atlasManager.createAtlas(\"game\", 1024, 1024);" << std::endl;
        std::cout << "auto uiAtlas = atlasManager.createAtlas(\"ui\", 512, 512);" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "// 2. Load sprites into appropriate atlases" << std::endl;
        std::cout << "renderSystem->loadSpriteToAtlas(\"player\", \"./assets/player.png\", \"game\");" << std::endl;
        std::cout << "renderSystem->loadSpriteToAtlas(\"enemy\", \"./assets/enemy.png\", \"game\");" << std::endl;
        std::cout << "renderSystem->loadSpriteToAtlas(\"button\", \"./assets/button.png\", \"ui\");" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "// 3. Generate all atlases (call after loading all sprites)" << std::endl;
        std::cout << "renderSystem->generateAtlases();" << std::endl;
        
        std::cout << "============================" << std::endl;
    }
    
}

// Example usage in main function
void runSpriteRenderingExample() {
    // This would typically be called from your main game loop
    SpriteRenderingExample example;
    example.setupExample();
    
    // Show migration information
    MigrationGuide::showLegacyVsNewComparison();
    MigrationGuide::showAtlasSetupExample();
}

/*
 * INTEGRATION INTO EXISTING GAME:
 * 
 * 1. Add these includes to your main game files:
 *    #include "ECS/systems/OptimizedRenderSystem2D.hpp"
 *    #include "TextureAtlas.hpp"
 *    #include "SpriteBatcher.hpp"
 * 
 * 2. Replace your existing render system registration:
 *    // OLD: auto renderSystem = ecs.registerSystem<RenderSystem2D>();
 *    // NEW:
 *    auto renderSystem = ecs.registerSystem<OptimizedRenderSystem2D>();
 *    renderSystem->init();
 * 
 * 3. Set up your texture atlases during game initialization:
 *    renderSystem->loadSpriteToAtlas("player", "./assets/player.png", "game");
 *    // ... load all your sprites
 *    renderSystem->generateAtlases();
 * 
 * 4. Update sprite creation calls to use atlas names instead of texture IDs
 * 
 * 5. Run benchmarks to verify performance improvements
 * 
 * The system is designed to be backwards compatible - existing sprites with
 * textureID will still render, just without batching benefits.
 */