#pragma once
#include "ECS/ECS.hpp"
#include "ECS/systems/OptimizedRenderSystem2D.hpp"
#include "TextureAtlas.hpp"
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

struct BenchmarkConfig {
    int numSprites = 1000;
    int atlasSize = 1024;
    bool enableBatching = true;
    bool enableCulling = true;
    float testDurationSeconds = 10.0f;
    std::string outputFile = "render_benchmark.csv";
    
    // Sprite distribution
    float worldWidth = 100.0f;
    float worldHeight = 100.0f;
    glm::vec2 spriteScale = glm::vec2(1.0f);
    
    // Camera movement for culling test
    bool movingCamera = true;
    float cameraSpeed = 5.0f;
};

struct BenchmarkResults {
    double averageFPS = 0.0;
    double minFPS = std::numeric_limits<double>::max();
    double maxFPS = 0.0;
    
    double averageFrameTime = 0.0;
    double minFrameTime = std::numeric_limits<double>::max();
    double maxFrameTime = 0.0;
    
    int totalFrames = 0;
    int averageDrawCalls = 0;
    int averageSpritesRendered = 0;
    int averageSpritesCulled = 0;
    int averageBatches = 0;
    
    double testDuration = 0.0;
    int configSprites = 0;
    bool batchingEnabled = false;
    bool cullingEnabled = false;
    
    void calculateAverages() {
        if (totalFrames > 0) {
            averageFPS = totalFrames / testDuration;
            averageFrameTime = testDuration / totalFrames * 1000.0; // Convert to ms
        }
    }
    
    void print() const {
        std::cout << "\n=== BENCHMARK RESULTS ===" << std::endl;
        std::cout << "Configuration:" << std::endl;
        std::cout << "  Sprites: " << configSprites << std::endl;
        std::cout << "  Batching: " << (batchingEnabled ? "ON" : "OFF") << std::endl;
        std::cout << "  Culling: " << (cullingEnabled ? "ON" : "OFF") << std::endl;
        std::cout << "  Duration: " << testDuration << "s" << std::endl;
        std::cout << "\nPerformance:" << std::endl;
        std::cout << "  Average FPS: " << averageFPS << std::endl;
        std::cout << "  Min FPS: " << minFPS << std::endl;
        std::cout << "  Max FPS: " << maxFPS << std::endl;
        std::cout << "  Average Frame Time: " << averageFrameTime << "ms" << std::endl;
        std::cout << "  Min Frame Time: " << minFrameTime << "ms" << std::endl;
        std::cout << "  Max Frame Time: " << maxFrameTime << "ms" << std::endl;
        std::cout << "\nRendering:" << std::endl;
        std::cout << "  Average Draw Calls: " << averageDrawCalls << std::endl;
        std::cout << "  Average Sprites Rendered: " << averageSpritesRendered << std::endl;
        std::cout << "  Average Sprites Culled: " << averageSpritesCulled << std::endl;
        std::cout << "  Average Batches: " << averageBatches << std::endl;
        std::cout << "========================\n" << std::endl;
    }
};

class RenderBenchmark {
public:
    RenderBenchmark() : generator(std::random_device{}()) {}
    
    BenchmarkResults runBenchmark(ECS& ecs, OptimizedRenderSystem2D& renderSystem, 
                                 const BenchmarkConfig& config = BenchmarkConfig{}) {
        std::cout << "Starting render benchmark with " << config.numSprites << " sprites..." << std::endl;
        
        // Set up benchmark environment
        setupBenchmark(ecs, renderSystem, config);
        
        // Configure render system
        renderSystem.enableBatching = config.enableBatching;
        renderSystem.enableFrustumCulling = config.enableCulling;
        
        // Run benchmark
        BenchmarkResults results = runBenchmarkLoop(ecs, renderSystem, config);
        
        // Clean up
        cleanupBenchmark(ecs);
        
        return results;
    }
    
    void runComparisonBenchmark(ECS& ecs, OptimizedRenderSystem2D& renderSystem) {
        std::cout << "Running comparison benchmark..." << std::endl;
        
        BenchmarkConfig config;
        config.numSprites = 1000;
        config.testDurationSeconds = 5.0f;
        
        // Test configurations
        std::vector<std::pair<std::string, BenchmarkConfig>> tests = {
            {"No Batching, No Culling", {config.numSprites, 1024, false, false, config.testDurationSeconds}},
            {"Batching Only", {config.numSprites, 1024, true, false, config.testDurationSeconds}},
            {"Culling Only", {config.numSprites, 1024, false, true, config.testDurationSeconds}},
            {"Batching + Culling", {config.numSprites, 1024, true, true, config.testDurationSeconds}},
        };
        
        std::vector<BenchmarkResults> results;
        
        for (const auto& test : tests) {
            std::cout << "\nTesting: " << test.first << std::endl;
            BenchmarkResults result = runBenchmark(ecs, renderSystem, test.second);
            result.print();
            results.push_back(result);
        }
        
        // Save comparison results
        saveComparisonResults(results, tests);
    }
    
    void runScalabilityTest(ECS& ecs, OptimizedRenderSystem2D& renderSystem) {
        std::cout << "Running scalability test..." << std::endl;
        
        std::vector<int> spriteCounts = {100, 250, 500, 750, 1000, 1500, 2000, 3000, 5000};
        std::vector<BenchmarkResults> results;
        
        for (int spriteCount : spriteCounts) {
            BenchmarkConfig config;
            config.numSprites = spriteCount;
            config.testDurationSeconds = 3.0f;
            config.enableBatching = true;
            config.enableCulling = true;
            
            std::cout << "\nTesting with " << spriteCount << " sprites..." << std::endl;
            BenchmarkResults result = runBenchmark(ecs, renderSystem, config);
            results.push_back(result);
            
            std::cout << "  FPS: " << result.averageFPS << ", Draw Calls: " << result.averageDrawCalls << std::endl;
            
            // Stop if performance drops too low
            if (result.averageFPS < 30.0) {
                std::cout << "Performance dropped below 30 FPS, stopping scalability test." << std::endl;
                break;
            }
        }
        
        saveScalabilityResults(results, spriteCounts);
    }

private:
    std::vector<size_t> benchmarkEntities;
    std::mt19937 generator;
    size_t cameraEntity;
    
    void setupBenchmark(ECS& ecs, OptimizedRenderSystem2D& renderSystem, const BenchmarkConfig& config) {
        // Create camera entity
        cameraEntity = ecs.createEntity();
        ecs.addComponent(cameraEntity, CameraComponent2D(glm::vec2(0.0f), 0.0f, 1.0f));
        renderSystem.cameraEntity = cameraEntity;
        
        // Load test sprite into atlas
        auto& atlasManager = TextureAtlasManager::getInstance();
        auto atlas = atlasManager.createAtlas("benchmark", config.atlasSize, config.atlasSize);
        
        // Create a simple colored square texture for testing
        createTestTexture();
        
        // Create sprites distributed across the world
        benchmarkEntities.clear();
        benchmarkEntities.reserve(config.numSprites);
        
        std::uniform_real_distribution<float> xDist(-config.worldWidth/2, config.worldWidth/2);
        std::uniform_real_distribution<float> yDist(-config.worldHeight/2, config.worldHeight/2);
        std::uniform_real_distribution<float> colorDist(0.5f, 1.0f);
        std::uniform_int_distribution<int> layerDist(0, 5);
        
        for (int i = 0; i < config.numSprites; ++i) {
            glm::vec3 position(xDist(generator), yDist(generator), 0.0f);
            glm::vec4 color(colorDist(generator), colorDist(generator), colorDist(generator), 1.0f);
            int layer = layerDist(generator);
            
            size_t entity = renderSystem.createSpriteEntity(ecs, "test_sprite", "benchmark", 
                                                          position, config.spriteScale, color, layer);
            benchmarkEntities.push_back(entity);
        }
        
        // Generate atlas
        renderSystem.generateAtlases();
        
        std::cout << "Benchmark setup complete: " << config.numSprites << " sprites created" << std::endl;
    }
    
    BenchmarkResults runBenchmarkLoop(ECS& ecs, OptimizedRenderSystem2D& renderSystem, const BenchmarkConfig& config) {
        BenchmarkResults results;
        results.configSprites = config.numSprites;
        results.batchingEnabled = config.enableBatching;
        results.cullingEnabled = config.enableCulling;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        auto lastFrameTime = startTime;
        
        // Benchmark variables
        int frameCount = 0;
        double totalDrawCalls = 0;
        double totalSpritesRendered = 0;
        double totalSpritesCulled = 0;
        double totalBatches = 0;
        
        float cameraX = 0.0f;
        float cameraY = 0.0f;
        
        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration<double>(currentTime - startTime).count();
            
            if (elapsed >= config.testDurationSeconds) {
                break;
            }
            
            auto frameDelta = std::chrono::duration<double>(currentTime - lastFrameTime).count();
            lastFrameTime = currentTime;
            
            // Update camera position for culling test
            if (config.movingCamera) {
                cameraX += config.cameraSpeed * frameDelta;
                cameraY += config.cameraSpeed * frameDelta * 0.5f;
                
                auto& camera = ecs.getComponent<CameraComponent2D>(cameraEntity);
                camera.position = glm::vec2(cameraX, cameraY);
                camera.updateMatrices();
            }
            
            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);
            
            // Reset render stats
            SpriteRenderManager::getInstance().resetStats();
            
            // Run render system
            renderSystem.update(frameDelta, ecs);
            
            // Collect statistics
            const auto& stats = SpriteRenderManager::getInstance().getStats();
            
            // Calculate FPS
            double fps = 1.0 / frameDelta;
            double frameTimeMs = frameDelta * 1000.0;
            
            // Update results
            results.minFPS = std::min(results.minFPS, fps);
            results.maxFPS = std::max(results.maxFPS, fps);
            results.minFrameTime = std::min(results.minFrameTime, frameTimeMs);
            results.maxFrameTime = std::max(results.maxFrameTime, frameTimeMs);
            
            totalDrawCalls += stats.drawCalls;
            totalSpritesRendered += stats.spritesRendered;
            totalSpritesCulled += stats.spritesCulled;
            totalBatches += stats.batchesCreated;
            
            frameCount++;
        }
        
        // Calculate final results
        results.totalFrames = frameCount;
        results.testDuration = config.testDurationSeconds;
        results.calculateAverages();
        
        if (frameCount > 0) {
            results.averageDrawCalls = static_cast<int>(totalDrawCalls / frameCount);
            results.averageSpritesRendered = static_cast<int>(totalSpritesRendered / frameCount);
            results.averageSpritesCulled = static_cast<int>(totalSpritesCulled / frameCount);
            results.averageBatches = static_cast<int>(totalBatches / frameCount);
        }
        
        return results;
    }
    
    void cleanupBenchmark(ECS& ecs) {
        // Remove all benchmark entities
        for (size_t entity : benchmarkEntities) {
            ecs.removeEntity(entity);
        }
        benchmarkEntities.clear();
        
        // Remove camera entity
        ecs.removeEntity(cameraEntity);
    }
    
    void createTestTexture() {
        // Create a simple 64x64 colored square for testing
        const int size = 64;
        std::vector<unsigned char> textureData(size * size * 4);
        
        for (int i = 0; i < size * size; ++i) {
            textureData[i * 4 + 0] = 255; // R
            textureData[i * 4 + 1] = 128; // G  
            textureData[i * 4 + 2] = 64;  // B
            textureData[i * 4 + 3] = 255; // A
        }
        
        // Add to atlas
        auto& atlasManager = TextureAtlasManager::getInstance();
        auto atlas = atlasManager.getAtlas("benchmark");
        if (atlas) {
            atlas->addSprite("test_sprite", textureData.data(), size, size, 4);
        }
    }
    
    void saveComparisonResults(const std::vector<BenchmarkResults>& results, 
                              const std::vector<std::pair<std::string, BenchmarkConfig>>& tests) {
        std::ofstream file("comparison_benchmark.csv");
        file << "Test,FPS,FrameTime(ms),DrawCalls,SpritesRendered,SpritesCulled,Batches\n";
        
        for (size_t i = 0; i < results.size() && i < tests.size(); ++i) {
            const auto& result = results[i];
            const auto& testName = tests[i].first;
            
            file << testName << "," 
                 << result.averageFPS << ","
                 << result.averageFrameTime << ","
                 << result.averageDrawCalls << ","
                 << result.averageSpritesRendered << ","
                 << result.averageSpritesCulled << ","
                 << result.averageBatches << "\n";
        }
        
        file.close();
        std::cout << "Comparison results saved to comparison_benchmark.csv" << std::endl;
    }
    
    void saveScalabilityResults(const std::vector<BenchmarkResults>& results, 
                               const std::vector<int>& spriteCounts) {
        std::ofstream file("scalability_benchmark.csv");
        file << "SpriteCount,FPS,FrameTime(ms),DrawCalls,SpritesRendered,SpritesCulled,Batches\n";
        
        for (size_t i = 0; i < results.size() && i < spriteCounts.size(); ++i) {
            const auto& result = results[i];
            int spriteCount = spriteCounts[i];
            
            file << spriteCount << "," 
                 << result.averageFPS << ","
                 << result.averageFrameTime << ","
                 << result.averageDrawCalls << ","
                 << result.averageSpritesRendered << ","
                 << result.averageSpritesCulled << ","
                 << result.averageBatches << "\n";
        }
        
        file.close();
        std::cout << "Scalability results saved to scalability_benchmark.csv" << std::endl;
    }
};