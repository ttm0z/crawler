#include "WorldGenerator.hpp"
#include <limits>
#include <math.h>
#include <iomanip>


WorldGenerator::WorldGenerator(unsigned int seed){};

// need a system to reconcile world and chunk coordinates

/** GenerateWorld
 *  1. Determine land vs water chunks
 * 
 * Returns -> 2D Vector of ChunkData (for world Object)
 * 
 */
std::vector<std::vector<ChunkData>> WorldGenerator::generateWorld(int width, int height) {
    std::cout << "[WorldGenerator]: Generating World . . . \n";
    std::vector<std::vector<ChunkData>> worldGrid(width, std::vector<ChunkData>(height));

    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float maxDistance = std::sqrt(centerX * centerX + centerY * centerY);

    // Parameters to control terrain generation
    float elevationScale = 15.0f;  // Reduced elevation scale to create more lowlands
    float noiseScale = 0.02f;      // Lower frequency for larger terrain features
    float riverScale = 0.03f;      // Increase frequency of rivers
    float lakeScale = 0.06f;       // Introduce lake patches

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            worldGrid[x][y] = generateChunkData(x, y, width, height);
        }
    }
    std::cout << "[WorldGenerator]: World Generated.\n";
    return worldGrid;
}


ChunkData WorldGenerator::generateChunkData(int x, int y, int width, int height) {
    
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    
    float dx = x - centerX;
    float dy = y - centerY;
    
    float maxDistance = std::sqrt(centerX * centerX + centerY * centerY);
    float distanceRatio = std::sqrt(dx * dx + dy * dy) / maxDistance;

    float noiseScale = 0.02f;
    float baseElevation = generateNoise(x * noiseScale, y * noiseScale, 8, 0.1f);
    float ridge = std::abs(1.0f - 2.0f * generateNoise(x * 0.01f, y * 0.01f, 4, 0.5f)); // Ridge noise
    float elevation = (baseElevation + ridge * 0.5f) * 15.0f + 2.5f;
    elevation *= (1.0f - distanceRatio * distanceRatio);

    float normLat = float(y) / height;
    float temperature = (1.0f - std::abs(normLat - 0.5f) * 2.0f);
    temperature *= (1.0f - elevation * 0.04f);
    temperature = glm::clamp(temperature, 0.0f, 1.0f);

    float oceanProximity = 1.0f - distanceRatio;
    float moisture = generateNoise(x * 0.015f, y * 0.015f, 4, 0.5f);
    moisture *= oceanProximity;
    moisture = glm::clamp(moisture, 0.0f, 1.0f);

    BiomeType biome;
    if (elevation <= 6.0f) biome = BiomeType::Ocean;
    else if (moisture < 0.2f && temperature > 0.6f) biome = BiomeType::Desert;
    else if (temperature > 0.7f && moisture > 0.6f) biome = BiomeType::Rainforest;
    else if (temperature > 0.5f) biome = BiomeType::Grassland;
    else if (temperature > 0.3f && moisture > 0.4f) biome = BiomeType::Taiga;
    else if (temperature < 0.3f) biome = BiomeType::Tundra;
    else biome = BiomeType::Forest;

    float fertility = moisture * (1.0f - elevation * 0.05f);
    fertility = glm::clamp(fertility, 0.0f, 1.0f);

    float vegetation = 0.0f;
    switch (biome) {
        case BiomeType::Rainforest: vegetation = fertility * 0.9f; break;
        case BiomeType::Forest: vegetation = fertility * 0.7f; break;
        case BiomeType::Grassland: vegetation = fertility * 0.5f; break;
        case BiomeType::Tundra: vegetation = fertility * 0.2f; break;
        default: vegetation = fertility * 0.1f; break;
    }

    ChunkData chunk;
    chunk.baseElevation = baseElevation;
    chunk.finalElevation = (int) elevation;
    
    chunk.temperature = temperature;
    chunk.moisture = moisture;
    chunk.biome = biome;
    chunk.fertility = fertility;
    chunk.vegetationDensity = vegetation;
    chunk.riverFlow = 0.0f; // To be implemented in river carving
    return chunk;
}

void WorldGenerator::printWorld(const std::vector<std::vector<ChunkData>>& world) {
    std::unordered_map<BiomeType, char> biomeSymbols = {
        {BiomeType::Ocean, '~'},
        {BiomeType::Beach, '.'},
        {BiomeType::Grassland, ','},
        {BiomeType::Forest, 'T'},
        {BiomeType::Rainforest, '@'},
        {BiomeType::Desert, '^'},
        {BiomeType::Tundra, '*'},
        {BiomeType::Taiga, '#'},
        {BiomeType::Swamp, '%'},
        {BiomeType::Mountain, 'M'},
        {BiomeType::Snow, 'S'}
    };

    int oceanCount = 0;
    int landCount = 0;
    float elevation = 0.0f;

    std::cout << "[WorldGenerator]: World Biome Map:\n";
    for (int y = world[0].size() - 1; y >= 0; --y) {  // Top-to-bottom display
        for (int x = 0; x < world.size(); ++x) {
            const ChunkData& data = world[x][y];
            if (data.finalElevation <= 0) {
                oceanCount++;
                std::cout << '~';
            } else {
                landCount++;
                elevation += data.finalElevation;
                auto it = biomeSymbols.find(data.biome);
                if (it != biomeSymbols.end()) {
                    std::cout << it->second;
                } else {
                    std::cout << '?';  // Unknown biome
                }
            }
        }
        std::cout << '\n';
    }

    std::cout << "[Stats] Oceans: " << oceanCount 
              << ", Land: " << landCount 
              << ", Avg Elevation: " << (landCount > 0 ? elevation / landCount : 0.0f)
              << "\n";
}
void WorldGenerator::printElevationMap(const std::vector<std::vector<ChunkData>>& world) {
    int width = world.size();
    int height = world[0].size();

    std::cout << "[WorldGenerator]: Elevation Map:\n";
    for (int y = height - 1; y >= 0; --y) {  // Top-to-bottom
        for (int x = 0; x < width; ++x) {
            const ChunkData& data = world[x][y];
            int elev = static_cast<int>(data.finalElevation);
            std::cout << std::setw(2) << std::setfill('0') << elev << ' ';
        }
        std::cout << '\n';
    }
}


// Calculate Euclidean distance between two points
float WorldGenerator::euclideanDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}