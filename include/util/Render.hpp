#pragma once
#include <vector>
#include "ECS/Archetypes.hpp"
#include "ECS/Components.hpp"
#include <cstdlib>   // for rand
#include <ctime>     // for time
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp> // if using GLM random (optional)
#include "GenerateNoise.hpp"
#include <random>

inline std::unordered_map<std::string, std::string> modelMap = {
    {"mammoth", "woolyMammoth"},
    {"human", "riggedFigure"}
};

inline std::vector<GLfloat> playerModelVertex = {
    // Head cube vertices (with texture coordinates)
    -0.2f,  0.8f,  0.2f, 0.0f, 0.0f,  0.2f,  0.8f,  0.2f, 1.0f, 0.0f,  0.2f, 1.2f,  0.2f, 1.0f, 1.0f, -0.2f, 1.2f,  0.2f, 0.0f, 1.0f,
    -0.2f,  0.8f, -0.2f, 0.0f, 0.0f,  0.2f,  0.8f, -0.2f, 1.0f, 0.0f,  0.2f, 1.2f, -0.2f, 1.0f, 1.0f, -0.2f, 1.2f, -0.2f, 0.0f, 1.0f,

    // Body vertices
    -0.3f,  0.2f,  0.1f, 0.0f, 0.0f,  0.3f,  0.2f,  0.1f, 1.0f, 0.0f,  0.3f, 0.8f,  0.1f, 1.0f, 1.0f, -0.3f, 0.8f,  0.1f, 0.0f, 1.0f,
    -0.3f,  0.2f, -0.1f, 0.0f, 0.0f,  0.3f,  0.2f, -0.1f, 1.0f, 0.0f,  0.3f, 0.8f, -0.1f, 1.0f, 1.0f, -0.3f, 0.8f, -0.1f, 0.0f, 1.0f,

    // Left Arm
    -0.45f, 0.6f,  0.05f, 0.0f, 0.0f, -0.3f, 0.6f,  0.05f, 1.0f, 0.0f, -0.3f, 0.2f,  0.05f, 1.0f, 1.0f, -0.45f, 0.2f,  0.05f, 0.0f, 1.0f,
    -0.45f, 0.6f, -0.05f, 0.0f, 0.0f, -0.3f, 0.6f, -0.05f, 1.0f, 0.0f, -0.3f, 0.2f, -0.05f, 1.0f, 1.0f, -0.45f, 0.2f, -0.05f, 0.0f, 1.0f,

    // Right Arm
    0.3f, 0.6f,  0.05f, 0.0f, 0.0f,  0.45f, 0.6f,  0.05f, 1.0f, 0.0f,  0.45f, 0.2f,  0.05f, 1.0f, 1.0f,  0.3f, 0.2f,  0.05f, 0.0f, 1.0f,
    0.3f, 0.6f, -0.05f, 0.0f, 0.0f,  0.45f, 0.6f, -0.05f, 1.0f, 0.0f,  0.45f, 0.2f, -0.05f, 1.0f, 1.0f,  0.3f, 0.2f, -0.05f, 0.0f, 1.0f,

    // Left Leg
    -0.25f, -0.4f,  0.05f, 0.0f, 0.0f, -0.05f, -0.4f,  0.05f, 1.0f, 0.0f, -0.05f, 0.2f,  0.05f, 1.0f, 1.0f, -0.25f, 0.2f,  0.05f, 0.0f, 1.0f,
    -0.25f, -0.4f, -0.05f, 0.0f, 0.0f, -0.05f, -0.4f, -0.05f, 1.0f, 0.0f, -0.05f, 0.2f, -0.05f, 1.0f, 1.0f, -0.25f, 0.2f, -0.05f, 0.0f, 1.0f,

    // Right Leg
    0.05f, -0.4f,  0.05f, 0.0f, 0.0f,  0.25f, -0.4f,  0.05f, 1.0f, 0.0f,  0.25f, 0.2f,  0.05f, 1.0f, 1.0f,  0.05f, 0.2f,  0.05f, 0.0f, 1.0f,
    0.05f, -0.4f, -0.05f, 0.0f, 0.0f,  0.25f, -0.4f, -0.05f, 1.0f, 0.0f,  0.25f, 0.2f, -0.05f, 1.0f, 1.0f,  0.05f, 0.2f, -0.05f, 0.0f, 1.0f
};

inline std::vector<GLuint> playerModelIndex = {
    // Head
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 4, 7, 7, 3, 0,
    1, 5, 6, 6, 2, 1,
    3, 2, 6, 6, 7, 3,
    0, 1, 5, 5, 4, 0,

    // Body
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    8, 12, 15, 15, 11, 8,
    9, 13, 14, 14, 10, 9,
    11, 10, 14, 14, 15, 11,
    8, 9, 13, 13, 12, 8,

    // Left Arm
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20,
    16, 20, 23, 23, 19, 16,
    17, 21, 22, 22, 18, 17,
    19, 18, 22, 22, 23, 19,
    16, 17, 21, 21, 20, 16,

    // Right Arm
    24, 25, 26, 26, 27, 24,
    28, 29, 30, 30, 31, 28,
    24, 28, 31, 31, 27, 24,
    25, 29, 30, 30, 26, 25,
    27, 26, 30, 30, 31, 27,
    24, 25, 29, 29, 28, 24,

    // Left Leg
    32, 33, 34, 34, 35, 32,
    36, 37, 38, 38, 39, 36,
    32, 36, 39, 39, 35, 32,
    33, 37, 38, 38, 34, 33,
    35, 34, 38, 38, 39, 35,
    32, 33, 37, 37, 36, 32,

    // Right Leg
    40, 41, 42, 42, 43, 40,
    44, 45, 46, 46, 47, 44,
    40, 44, 47, 47, 43, 40,
    41, 45, 46, 46, 42, 41,
    43, 42, 46, 46, 47, 43,
    40, 41, 45, 45, 44, 40
};
inline std::vector<GLfloat> sphereVertices = {
    // positions (x, y, z)   // texture coords (u, v)
    0.0f,  0.5f,  0.0f,      0.5f, 1.0f,   // top
    0.35f,  0.35f,  0.35f,   0.85f, 0.85f,
    0.5f,  0.0f,  0.0f,      1.0f, 0.5f,
    0.35f,  0.35f, -0.35f,   0.85f, 0.15f,
    0.0f,  0.0f,  0.5f,      0.5f, 0.5f,
    0.0f,  0.0f, -0.5f,      0.5f, 0.0f,
   -0.35f,  0.35f,  0.35f,   0.15f, 0.85f,
   -0.5f,  0.0f,  0.0f,      0.0f, 0.5f,
   -0.35f,  0.35f, -0.35f,   0.15f, 0.15f,
    0.0f, -0.5f,  0.0f,      0.5f, 0.0f,  // bottom
   -0.35f, -0.35f,  0.35f,   0.15f, 0.65f,
    0.35f, -0.35f,  0.35f,   0.85f, 0.65f,
    0.35f, -0.35f, -0.35f,   0.85f, 0.35f,
   -0.35f, -0.35f, -0.35f,   0.15f, 0.35f
};

inline std::vector<GLuint> sphereIndices = {
    // upper hemisphere
    0, 1, 4,
    0, 4, 6,
    0, 6, 7,
    0, 7, 3,
    0, 3, 1,
    1, 2, 4,
    1, 3, 2,
    4, 2, 5,
    6, 4, 5,
    7, 6, 8,
    3, 7, 8,
    2, 3, 8,

    // lower hemisphere
    9, 10, 8,
    10, 11, 8,
    11, 12, 8,
    12, 9, 8,
    9, 12, 5,
    9, 5, 10,
    10, 5, 2,
    10, 2, 11,
    11, 2, 12,
    12, 2, 5
};


inline int getTextureIndex(TileType type) {
    switch (type) {
        case TileType::WATER: return 0;
        case TileType::SAND:  return 1;
        case TileType::GRASS: return 2;
        case TileType::DIRT:  return 3;
        case TileType::ROCK:  return 4;
        default: return 0; // Default to WATER if undefined
    }
}
inline std::vector<GLfloat> hexagonVertices = {
        // Bottom face vertices (y = -0.5)
         0.5f,  0.0f ,
         0.25f, 0.433,
        -0.25f, 0.433,
        -0.5f,  0.0f ,
        -0.25f, -0.433,
         0.25f, -0.433,

        // Top face vertices (y = 0.5)
         0.5f,  0.0f, 
         0.25f, 0.433f,
        -0.25f, 0.433f, 
        -0.5f,  0.0f, 
        -0.25f, -0.433f,
         0.25f, -0.433f
    };

inline glm::vec3 getTilePosition(int q, int r, float hexRadius) {
    
    float xOffset = hexRadius * 3.0f / 2.0f * 0.5f; 
    float yOffset = hexRadius * sqrt(3.0f) * 0.5f;  

    
    float x = q * xOffset; // x-coordinate is the regular offset
    float y = r * yOffset; // y-coordinate is the regular offset

    
    if (q % 2 != 0) {
        y += yOffset / 2.0f;
    }

    return glm::vec3(x, 0.0f, y); // Return the final position in world space (xz-plane)
}
inline float fractalNoise(float x, float y, int octaves, float persistence, float lacunarity) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        total += generateNoise(x * frequency, y * frequency, 3, 0.1f) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return total / maxValue; // Normalized
}


inline void generateChunkGeometry(ChunkComponent& chunk, std::vector<std::vector<ChunkData>> map,
                               std::vector<GLfloat>& vertexData,
                               std::vector<GLint>& indices)
{
    vertexData.clear();
    indices.clear();
    
    float tileHeight = chunk.hexRadius / 2.0f; // Height of the hex tile
    
    auto pushVertex = [&](const glm::vec3& pos, const glm::vec2& texCoord, const float texIndex) {
        //std::cout << "\tVertex:" << pos.x << " "<<  pos.y << " " << pos.z << " \n";
        vertexData.push_back(pos.x);
        vertexData.push_back(pos.y);
        vertexData.push_back(pos.z);
        vertexData.push_back(texCoord.x);
        vertexData.push_back(texCoord.y);
        vertexData.push_back(texIndex);
    };

    glm::vec2 texCoords[6] = {
        {0.5f, 1.0f}, {1.0f, 0.75f}, {1.0f, 0.25f},
        {0.5f, 0.0f}, {0.0f, 0.25f}, {0.0f, 0.75f}
    };

    glm::vec2 sideTexCoords[6] = {
        {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f},  // Square for each side face
    };


    for (int layer = 0; layer < chunk.depth; ++layer) {
        for (int r = 0; r < chunk.columns; ++r) {
            for (int q = 0; q < chunk.rows; ++q) {
                int tileIndex = chunk.indexFrom3D(q, r, layer);
                HexTile& tile = chunk.tiles[tileIndex];
                glm::vec3 center = getTilePosition(q, r, chunk.hexRadius);
                
                
                float xOffset = chunk.hexRadius * 3.0f / 2.0f; 
                float yOffset = chunk.hexRadius * sqrt(3.0f) * 0.5f;
                
                // Generate tile type
                float tileValue = generateNoise(r, q, 5, 0.2f);
                int idx;
                if(tileValue < 0.5f){
                    idx = 0;
                }
                else if (tileValue < 0.7f) {
                    idx = std::min(1, (int)chunk.palette.size());
                }
                else{
                    idx = std::min(2, (int)chunk.palette.size());
                }

                tile.texIndex = getTextureIndex(chunk.palette[idx]);
                center.x += (float)chunk.chunkIndex[0] * ((float) (chunk.columns) * xOffset / 2.0f);
                center.z += (float)chunk.chunkIndex[1] * ((float) (chunk.rows) * yOffset);
                center.y += (float)chunk.chunkIndex[2] * (float)chunk.depth * tileHeight;
                
                //generate heightvalue
                
                float noiseScale = 0.45f;
                //float heightValue = chunk.elevation * 3.0f * fractalNoise(r * noiseScale, q * noiseScale, 5, 0.5f, 2.0f);
                float heightValue = 3;

                
                int gridX = chunk.chunkIndex[0] + (map[0].size() / 2);
                int gridY = chunk.chunkIndex[1] + (map.size() / 2);
                float minHeight = heightValue;
                
                // Create Beach
                int adjacentWaterTiles = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int nx = gridX + dx;
                        int ny = gridY + dy;
                        if (nx > 0 && nx < map.size() && ny > 0 && ny < map[0].size()) {
                            if (map[nx][ny].finalElevation == 0) {
                                adjacentWaterTiles++;
                                minHeight = std::min(minHeight, 5.0f);
                            }
                        }
                    }
                }
                if (adjacentWaterTiles > 0) { 
                    tile.texIndex = getTextureIndex(TileType::SAND);
                    if (heightValue <= 5.0f) {
                        tile.texIndex = getTextureIndex(TileType::WATER);
                        heightValue = 5.0f;
                    }
                }
                if (chunk.elevation == 2) { 
                    if (heightValue < 5.0f) {
                        tile.texIndex = getTextureIndex(TileType::WATER);
                        heightValue = 5.0f;
                    }
                }
                // set water tiles
                if(chunk.elevation == 0){
                    heightValue = 5;
                    tile.texIndex = getTextureIndex(TileType::WATER);
                }
                
                // (for testing structures and models)
                
                if(layer < heightValue)
                    {
                        chunk.occupancy[tileIndex] = 1;                        
                    }
                else chunk.occupancy[tileIndex] = 0;


                // Generate Chunk Geometry
                if(chunk.occupancy[tileIndex] == 1){
                    GLint baseIndex = static_cast<GLint>(vertexData.size() / 6);

                    float hz = tileHeight * (layer + 1);
                    float hz2 = (tileHeight * layer);
                    // std::cout << "Tile at :" << q << " " << r << " " << layer <<"\n";
                    // std::cout << "\tCenter:" << center.x << " " << hz2 + (hz - hz2) / 2.0f << " " << center.z <<"\n";
                    // Top Face
                    glm::vec3 topCenter = center + glm::vec3(0.0f, hz, 0.0f);
                    pushVertex(topCenter, {0.5f, 0.5f}, static_cast<float>(tile.texIndex)); // Center UV

                    std::vector<GLint> topPerimeterIndices;
                    for (int i = 0; i < 6; ++i) {
                        float hx = hexagonVertices[i * 2] * chunk.hexRadius;
                        float hy = hexagonVertices[i * 2 + 1] * chunk.hexRadius;

                        glm::vec3 pos = center + glm::vec3(hx, hz, hy);
                        pushVertex(pos, texCoords[i], static_cast<float>(tile.texIndex));
                        topPerimeterIndices.push_back(static_cast<GLint>(vertexData.size() / 6 - 1));
                    }
                
                    for (int i = 0; i < 6; ++i) {
                        indices.push_back(topPerimeterIndices[(i + 1) % 6]);
                        indices.push_back(topPerimeterIndices[i]);
                        indices.push_back(baseIndex);
                        
                        
                    }

                    // Bottom Face
                    GLint bottomBase = static_cast<GLint>(vertexData.size() /6);
                    glm::vec3 bottomCenter = center + glm::vec3(0.0f, hz2, 0.0f);
                    pushVertex(bottomCenter, {0.5f, 0.5f}, static_cast<float>(tile.texIndex));

                    std::vector<GLint> bottomPerimeterIndices;
                    for (int i = 0; i < 6; ++i) {
                        float hx = hexagonVertices[i * 2] * chunk.hexRadius;
                        float hy = hexagonVertices[i * 2 + 1] * chunk.hexRadius;
                        glm::vec3 pos = center + glm::vec3(hx, hz2, hy);

                        pushVertex(pos, texCoords[i], static_cast<float>(tile.texIndex));
                        bottomPerimeterIndices.push_back(static_cast<GLint>(vertexData.size() / 6 - 1));
                    }

                    for (int i = 0; i < 6; ++i) {
                        indices.push_back(bottomBase);
                        indices.push_back(bottomPerimeterIndices[i]);
                        indices.push_back(bottomPerimeterIndices[(i + 1) % 6]);
                        
                        

                    }

                    // Side Faces
                    std::vector<GLint> sideTopIndices(6), sideBottomIndices(6);
                    for (int i = 0; i < 6; ++i) {
                        float hx = hexagonVertices[i * 2] * chunk.hexRadius;
                        float hy = hexagonVertices[i * 2 + 1] * chunk.hexRadius;

                        // Top and bottom positions for the side face
                        glm::vec3 posTop = center + glm::vec3(hx, hz, hy);
                        glm::vec3 posBottom = center + glm::vec3(hx, hz2, hy);

                        // Assign unique texture coordinates for top and bottom faces
                        glm::vec2 texCoordTop = sideTexCoords[i % 4];    // Use one of the four tex coordinates for the top
                        glm::vec2 texCoordBottom = sideTexCoords[(i + 2) % 4]; // Use one of the four tex coordinates for the bottom

                        pushVertex(posTop, texCoordTop, static_cast<float>(tile.texIndex));
                        sideTopIndices[i] = static_cast<GLint>(vertexData.size() / 6 - 1);
                        pushVertex(posBottom, texCoordBottom, static_cast<float>(tile.texIndex));
                        sideBottomIndices[i] = static_cast<GLint>(vertexData.size() / 6 - 1);
                    }

                    // Create the side faces using the new texture coordinates
                    for (int i = 0; i < 6; ++i) {
                        int next = (i + 1) % 6;
                        GLint v0 = sideTopIndices[i];
                        GLint v1 = sideTopIndices[next];
                        GLint v2 = sideBottomIndices[next];
                        GLint v3 = sideBottomIndices[i];
                        indices.push_back(v0);
                        indices.push_back(v1);
                        indices.push_back(v2);
                        indices.push_back(v0);
                        indices.push_back(v2);
                        indices.push_back(v3);


                    }
                
                }
            }
        }
    }
}


inline void updateChunkGeometry(ChunkComponent& chunk, RenderableComponent& renderable,
                                std::vector<GLfloat>& vertexData,
                                std::vector<GLint>& indices)
{
    vertexData.clear();
    indices.clear();
    
    float tileHeight = chunk.hexRadius / 2.0f;
    float xOffset = chunk.hexRadius * 3.0f / 2.0f; 
    float yOffset = chunk.hexRadius * sqrt(3.0f) * 0.5f;

    auto pushVertex = [&](const glm::vec3& pos, const glm::vec2& texCoord, float texIndex) {
        vertexData.push_back(pos.x);
        vertexData.push_back(pos.y);
        vertexData.push_back(pos.z);
        vertexData.push_back(texCoord.x);
        vertexData.push_back(texCoord.y);
        vertexData.push_back(texIndex);
    };

    glm::vec2 texCoords[6] = {
        {0.5f, 1.0f}, {1.0f, 0.75f}, {1.0f, 0.25f},
        {0.5f, 0.0f}, {0.0f, 0.25f}, {0.0f, 0.75f}
    };

    glm::vec2 sideTexCoords[4] = {
        {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}
    };

    for (int layer = 0; layer < chunk.depth; ++layer) {
        for (int r = 0; r < chunk.columns; ++r) {
            for (int q = 0; q < chunk.rows; ++q) {
                int tileIndex = chunk.indexFrom3D(q, r, layer);
                if (chunk.occupancy[tileIndex] != 1) continue;

                HexTile& tile = chunk.tiles[tileIndex];
                glm::vec3 center = getTilePosition(q, r, chunk.hexRadius);

                center.x += chunk.chunkIndex[0] * (chunk.columns * xOffset / 2.0f);
                center.z += chunk.chunkIndex[1] * (chunk.rows * yOffset);
                center.y += chunk.chunkIndex[2] * (chunk.depth * tileHeight);

                float hz = tileHeight * (layer + 1);
                float hz2 = tileHeight * layer;

                GLint baseIndex = static_cast<GLint>(vertexData.size() / 6);

                // Top Face
                glm::vec3 topCenter = center + glm::vec3(0.0f, hz, 0.0f);
                pushVertex(topCenter, {0.5f, 0.5f}, tile.texIndex);

                std::vector<GLint> topPerimeter;
                for (int i = 0; i < 6; ++i) {
                    float hx = hexagonVertices[i * 2] * chunk.hexRadius;
                    float hy = hexagonVertices[i * 2 + 1] * chunk.hexRadius;
                    glm::vec3 pos = center + glm::vec3(hx, hz, hy);
                    pushVertex(pos, texCoords[i], tile.texIndex);
                    topPerimeter.push_back(static_cast<GLint>(vertexData.size() / 6 - 1));
                }

                for (int i = 0; i < 6; ++i) {
                    indices.push_back(topPerimeter[(i + 1) % 6]);
                    indices.push_back(topPerimeter[i]);
                    indices.push_back(baseIndex);
                    

                }

                // Bottom Face
                GLint bottomBase = static_cast<GLint>(vertexData.size() / 6);
                glm::vec3 bottomCenter = center + glm::vec3(0.0f, hz2, 0.0f);
                pushVertex(bottomCenter, {0.5f, 0.5f}, tile.texIndex);

                std::vector<GLint> bottomPerimeter;
                for (int i = 0; i < 6; ++i) {
                    float hx = hexagonVertices[i * 2] * chunk.hexRadius;
                    float hy = hexagonVertices[i * 2 + 1] * chunk.hexRadius;
                    glm::vec3 pos = center + glm::vec3(hx, hz2, hy);
                    pushVertex(pos, texCoords[i], tile.texIndex);
                    bottomPerimeter.push_back(static_cast<GLint>(vertexData.size() / 6 - 1));
                }

                for (int i = 0; i < 6; ++i) {
                    
                    indices.push_back(bottomBase);
                    indices.push_back(bottomPerimeter[i]);
                    indices.push_back(bottomPerimeter[(i + 1) % 6]);

                }

                // Side Faces
                std::vector<GLint> sideTop(6), sideBottom(6);
                for (int i = 0; i < 6; ++i) {
                    float hx = hexagonVertices[i * 2] * chunk.hexRadius;
                    float hy = hexagonVertices[i * 2 + 1] * chunk.hexRadius;

                    glm::vec3 posTop = center + glm::vec3(hx, hz, hy);
                    glm::vec3 posBottom = center + glm::vec3(hx, hz2, hy);

                    glm::vec2 texCoordTop = sideTexCoords[i % 4];
                    glm::vec2 texCoordBottom = sideTexCoords[(i + 2) % 4];

                    pushVertex(posTop, texCoordTop, tile.texIndex);
                    sideTop[i] = static_cast<GLint>(vertexData.size() / 6 - 1);
                    pushVertex(posBottom, texCoordBottom, tile.texIndex);
                    sideBottom[i] = static_cast<GLint>(vertexData.size() / 6 - 1);
                }

                for (int i = 0; i < 6; ++i) {
                    int next = (i + 1) % 6;
                    GLint v0 = sideTop[i];
                    GLint v1 = sideTop[next];
                    GLint v2 = sideBottom[next];
                    GLint v3 = sideBottom[i];

                    indices.push_back(v0);
                    indices.push_back(v1);
                    indices.push_back(v2);
                    indices.push_back(v0);
                    indices.push_back(v2);
                    indices.push_back(v3);


                }
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, renderable.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);

    renderable.vertexCount = indices.size();
}



inline void initializeSkyboxRenderable(RenderableComponent& renderableComponent, SkyboxComponent& skybox) {
    // Generate VAO, VBO if they aren't already initialized
    if (renderableComponent.VAO == 0) {
        glGenVertexArrays(1, &renderableComponent.VAO);
    }
    if (renderableComponent.VBO == 0) {
        glGenBuffers(1, &renderableComponent.VBO);
    }

    glBindVertexArray(renderableComponent.VAO);

    // Bind and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, renderableComponent.VBO);
    glBufferData(GL_ARRAY_BUFFER, skybox.skyboxVertices.size() * sizeof(GLfloat), skybox.skyboxVertices.data(), GL_STATIC_DRAW);

    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Assign the shader
    renderableComponent.shader = ResourceManager<Shader>::getInstance().get(skybox.skyboxShader);
    if (!renderableComponent.shader) {
        std::cerr << "Failed to load skybox shader " << skybox.skyboxShader << " for RenderableComponent.\n";
    }
    renderableComponent.textureID = ResourceManager<Texture>::getInstance().get(skybox.cubemapId)->getID();
}

inline size_t addChunk(ECS* ecs, std::vector<std::vector<ChunkData>> map, ChunkComponent& chunk){
    auto startingChunk = ecs -> createEntity();
    ecs -> assignArchetype(startingChunk, {typeid(RenderableComponent).hash_code(), typeid(TransformComponent).hash_code()});
    ecs -> assignArchetype(startingChunk, dungeonRoomArchetype);
    TransformComponent chunkTransform;
    //chunkTransform.position += chunk.chunkIndex;
    std::string shader = "chunkShader";
    std::vector<GLfloat> vertices;
    std::vector<GLint> indices;
    RenderableComponent chunkRenderable;
    chunkRenderable.shader = ResourceManager<Shader>::getInstance().get(shader);

    if(chunk.elevation == 0){
        chunk.palette = {TileType::WATER};
    }
    if(chunk.elevation == 1){
        chunk.palette = {TileType::SAND, TileType::DIRT};
    }
    if(chunk.elevation == 2){
        chunk.palette = {TileType::DIRT, TileType::ROCK};
    }
    if(chunk.elevation > 2){
        chunk.palette = {TileType::DIRT, TileType::ROCK};
    }
    chunkRenderable.textureID = ResourceManager<Texture>::getInstance().get("chunkArray") -> getID();
    
    generateChunkGeometry(chunk, map, vertices, indices);
                    
    // Generate buffers
    glGenVertexArrays(1, &chunkRenderable.VAO);
    glGenBuffers(1, &chunkRenderable.VBO);
    glGenBuffers(1, &chunkRenderable.EBO);
    // Bind and fill the VAO/VBO/EBO
    glBindVertexArray(chunkRenderable.VAO);
    // Vertex Buffer: upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, chunkRenderable.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    // Element Buffer: upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkRenderable.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
    // Setup vertex attributes
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    
    glEnableVertexAttribArray(1); // Texture Coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    glEnableVertexAttribArray(2); // Texture Index
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    
    // Unbind VAO
    glBindVertexArray(0);
    // Save the total index count in the component (for glDrawElements)
    chunkRenderable.vertexCount = indices.size();
    
    ecs -> addComponent(startingChunk, chunk);
    ecs -> addComponent(startingChunk, chunkRenderable);
    ecs -> addComponent(startingChunk, chunkTransform);
    return startingChunk;
}


inline void initializeRenderableComponent(
    RenderableComponent& renderableComponent,
    std::vector<GLfloat> vertices,
    std::vector<GLuint> indices,
    std::string shaderCode,
    std::string textureCode) {
    // Generate VAO, VBO, and EBO if they aren't already initialized
    if (renderableComponent.VAO == 0) {
        glGenVertexArrays(1, &renderableComponent.VAO);
    }
    if (renderableComponent.VBO == 0) {
        glGenBuffers(1, &renderableComponent.VBO);
    }
    if (renderableComponent.EBO == 0) {
        glGenBuffers(1, &renderableComponent.EBO);
    }

    glBindVertexArray(renderableComponent.VAO);

    // Bind and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, renderableComponent.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Bind and upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderableComponent.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Configure vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Assign the shader
    renderableComponent.shader = ResourceManager<Shader>::getInstance().get(shaderCode);
    if (!renderableComponent.shader) {
        std::cerr << "Failed to load shader " << shaderCode << " for RenderableComponent.\n";
    }
    renderableComponent.textureID = ResourceManager<Texture>::getInstance().get(textureCode) -> getID();
}

inline void cleanupRenderableComponent(RenderableComponent& renderableComponent) {
        glDeleteVertexArrays(1, &renderableComponent.VAO);
        glDeleteBuffers(1, &renderableComponent.VBO);
        renderableComponent.VAO = 0;
        renderableComponent.VBO = 0;
    }


inline void generatePlayerGeometry(std::vector<GLfloat>& vertexData, std::vector<GLint>& indices) {
    vertexData.clear();
    indices.clear();

    // Define player geometry (a simple cube for now)
    std::vector<GLfloat> playerVertices = {
        // Positions        // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Back bottom left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // Back bottom right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Back top right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Back top left

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Front bottom left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Front bottom right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Front top right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f   // Front top left
    };

    std::vector<GLint> playerIndices = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 3, 7, 7, 4, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Bottom face
        0, 1, 5, 5, 4, 0,
        // Top face
        3, 2, 6, 6, 7, 3
    };

    // Store vertex data
    vertexData = playerVertices;
    indices = playerIndices;
}
inline void initializePlayerRenderable(RenderableComponent& renderableComponent) {

    
    renderableComponent.shader = ResourceManager<Shader>::getInstance().get("playerShader");
    renderableComponent.textureID = ResourceManager<Texture>::getInstance().get("playerTexture") -> getID();

    std::vector<GLuint> playerIndices = playerModelIndex;
    std::vector<GLfloat> playerVertices = playerModelVertex;
    // Generate buffers
    glGenVertexArrays(1, &renderableComponent.VAO);
    glGenBuffers(1, &renderableComponent.VBO);
    glGenBuffers(1, &renderableComponent.EBO);
    // Bind and fill the VAO/VBO/EBO
    glBindVertexArray(renderableComponent.VAO);
    // Vertex Buffer: upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, renderableComponent.VBO);
    glBufferData(GL_ARRAY_BUFFER, playerVertices.size() * sizeof(GLfloat), playerVertices.data(), GL_STATIC_DRAW);
    // Element Buffer: upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderableComponent.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, playerIndices.size() * sizeof(GLint), playerIndices.data(), GL_STATIC_DRAW);
    // Setup vertex attributes
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    
    glEnableVertexAttribArray(1); // Texture Coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // Unbind VAO
    glBindVertexArray(0);
    // Save the total index count in the component (for glDrawElements)
    renderableComponent.vertexCount = playerIndices.size();
}
/*
inline void initializeStaticRenderable(RenderableComponent& renderableComponent, std::string modelKey) {
    renderableComponent.shader = ResourceManager<Shader>::getInstance().get("treeShader");
    renderableComponent.textureID = ResourceManager<Texture>::getInstance().get("playerTexture") -> getID();
    std::cout << "Initializing Model: " << modelKey << "\n";
    renderableComponent.m_model = ResourceManager<Model>::getInstance().get(modelKey);
    std::cout << "Model Intiialized: " << modelKey << "\n";
}
*/



/*
inline void initializeNPC(RenderableComponent& renderableComponent, std::string modelKey) {
    renderableComponent.shader = ResourceManager<Shader>::getInstance().get("modelShader");
    renderableComponent.textureID = ResourceManager<Texture>::getInstance().get("playerTexture") -> getID();
    std::cout << "Initializing npc: " << modelKey << "\n";
    renderableComponent.m_model = ResourceManager<Model>::getInstance().get(modelKey);
}
*/


inline float randFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

/*
inline size_t addLightSource(ECS* ecs, glm::vec3 position, LightType type) {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    auto lightSource = ecs->createEntity();
    ecs->assignArchetype(lightSource, lightArchetype);

    LightSourceComponent2D light;
    light.position = position;

    // 2D lights don't need direction
    else {
        light.direction = glm::normalize(glm::vec3(
            randFloat(-1.0f, 1.0f),
            randFloat(-1.0f, 1.0f),
            randFloat(-1.0f, 1.0f)
        ));
    }

    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse = glm::vec3(1.0f, 1.0f, 0.90f); // slight yellow tint
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Attenuation parameters for point and spot lights
    light.constant = 1.0f;
    light.linear = randFloat(0.05f, 0.2f);
    light.quadratic = randFloat(0.01f, 0.05f);

    if (type == LightType::Spot) {
        float innerAngle = randFloat(10.0f, 20.0f);
        float outerAngle = innerAngle + randFloat(2.5f, 7.5f);
        light.innerCutOff = glm::cos(glm::radians(innerAngle));
        light.outerCutOff = glm::cos(glm::radians(outerAngle));
    }

    light.enabled = true;

    ecs->addComponent(lightSource, light);

    return lightSource;
}
*/



inline std::string generateStaticObjectModelID(StaticObject objectType){
    static const std::unordered_map<StaticObject, std::vector<std::string>> modelOptions = {
        { StaticObject::TREE, {"tree1", "tree2", "tree3", "tree4"} },
        { StaticObject::ROCK, {"rock1", "rock2", "rock3"} }
    };

    auto it = modelOptions.find(objectType);
    if (it != modelOptions.end()) {
        const std::vector<std::string>& options = it->second;

        // Choose a random index
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, options.size() - 1);

        return options[dis(gen)];
    }

    return "unknown_model";
}
inline size_t addStaticObject(ECS* ecs, glm::vec3 position, StaticObject objectType) {
            // Create Test Tree
        auto tree = ecs -> createEntity();
        ecs -> assignArchetype(tree, renderableArchetype);
        ecs -> assignArchetype(tree, staticObjectArchetype);

        StaticObjectComponent treeStaticComponent;
        RenderableComponent treeRenderable;
        std::string id = generateStaticObjectModelID(objectType);
        // initializeStaticRenderable(treeRenderable, id); // Commented out for 2D conversion
        TransformComponent treeTransform;
        treeTransform.scale = glm::vec3(0.5f);
        treeTransform.position = glm::vec3(position.x, position.y, position.z);
        if (id == "tree3")
            treeTransform.position.y -= 3.0f;

        ecs -> addComponent(tree, treeRenderable);
        ecs -> addComponent(tree, treeTransform);
        ecs -> addComponent(tree, treeStaticComponent);
        return tree;
}


inline size_t addNPC(ECS* ecs, glm::vec3 startingPosition, std::string modelID, std::string name, std::string race, int level, NPCState attitude) {
    
    auto npcEntity = ecs -> createEntity();
    ecs -> assignArchetype(npcEntity, npcArchetype);
    ecs -> assignArchetype(npcEntity, renderableArchetype);
    ecs -> assignArchetype(npcEntity, physicsArchetype);
    // ecs -> assignArchetype(npcEntity, animationArchetype); // Commented out for 2D conversion
    
    TransformComponent npcTransform;
    npcTransform.position = startingPosition;
    
    npcTransform.scale = glm::vec3(0.5f);    
    
    RenderableComponent npcRenderable;
    SkeletonComponent npcSkeleton;
    AnimationComponent npcAnimation;
    npcAnimation.currentAnimation = "Idle_01";
    NPCComponent npcComponent;
    npcComponent.name = name;
    npcComponent.level = level;
    npcComponent.race = race;
    npcComponent.attitude = attitude;

    PhysicsComponent2D npcPhysics;
    ColliderComponent2D npcCollider;
    npcCollider.size = glm::vec2(0.3f, 3.0f);
    
    auto modelKey = modelMap[modelID];
    
    // Commented out for 2D conversion
    // initializeNPC(npcRenderable, modelKey);
    // npcAnimation.animationMap = npcRenderable.m_model -> populateAnimationMap();
    // size_t animationIndex = npcAnimation.animationMap[npcAnimation.currentAnimation];
    // npcSkeleton.boneMatrices = npcRenderable.m_model -> calculateFinalBoneMatrices(0.0f, animationIndex);
    
    
    ecs -> addComponent(npcEntity, npcTransform);
    ecs -> addComponent(npcEntity, npcPhysics);
    ecs -> addComponent(npcEntity, npcComponent);
    ecs -> addComponent(npcEntity, npcRenderable);
    ecs -> addComponent(npcEntity, npcCollider);
    ecs -> addComponent(npcEntity, npcSkeleton);
    ecs -> addComponent(npcEntity, npcAnimation);
    
    return npcEntity;
}


inline glm::ivec2 worldToChunkCoord(const glm::vec3& worldPos, int chunkSize, float hexRadius) {
    float hexWidth = 2.0f * hexRadius;
    float hexHeight = sqrt(3.0f) * hexRadius;

    // Convert world position to axial coordinates first
    float q = (2.0f / 3.0f) * worldPos.x / (hexRadius*0.5f);
    float r = (-1.0f / 3.0f * worldPos.x + sqrt(3.0f) / 3.0f * worldPos.z) / (hexRadius*0.5f);

    // Convert axial to cube coords, then round to nearest hex
    float x = q;
    float z = r;
    float y = -x - z;

    int rx = round(x);
    int ry = round(y);
    int rz = round(z);

    float x_diff = abs(rx - x);
    float y_diff = abs(ry - y);
    float z_diff = abs(rz - z);

    if (x_diff > y_diff && x_diff > z_diff) rx = -ry - rz;
    else if (y_diff > z_diff) ry = -rx - rz;
    else rz = -rx - ry;

    // Convert cube to offset coordinates (odd-q vertical layout)
    int col = rx;
    int row = rz + (rx - (rx & 1)) / 2;

    // Find chunk coordinates
    int chunkX = floor((float)col / chunkSize);
    int chunkZ = floor((float)row / chunkSize);

    return glm::ivec2(chunkX, chunkZ);
}

    
inline glm::ivec3 worldToLocalTileCoord(const glm::vec3& worldPos, const glm::ivec2& chunkCoord, int chunkSize, float hexRadius) {
    float hexWidth = 2.0f * (hexRadius * 0.5f);
    float hexHeight = sqrt(3.0f) * (hexRadius * 0.5f);
    float tileHeight = hexRadius / 2.0f;
    // First, axial coordinates:
    float q = (2.0f / 3.0f) * worldPos.x / (hexRadius * 0.5f);
    float r = (-1.0f / 3.0f * worldPos.x + sqrt(3.0f) / 3.0f * worldPos.z) / (hexRadius*0.5f);

    // Cube rounding:
    float x = q;
    float z = r;
    float y = -x - z;

    int rx = round(x);
    int ry = round(y);
    int rz = round(z);

    float x_diff = abs(rx - x);
    float y_diff = abs(ry - y);
    float z_diff = abs(rz - z);

    if (x_diff > y_diff && x_diff > z_diff) rx = -ry - rz;
    else if (y_diff > z_diff) ry = -rx - rz;
    else rz = -rx - ry;

    // Convert cube coords to offset (odd-q vertical layout)
    int col = rx;
    int row = rz + (rx - (rx & 1)) / 2;

    // Get local coordinates within chunk
    int localX = col - (chunkCoord.x * chunkSize);
    int localZ = row - (chunkCoord.y * chunkSize);

    // Clamp or wrap, depending on needs
    localX = glm::clamp(localX, 0, chunkSize - 1);
    localZ = glm::clamp(localZ, 0, chunkSize - 1);
    int localY = static_cast<int>(floor(worldPos.y / tileHeight));

    return glm::ivec3(localX, localZ, localY);
}
inline glm::vec3 chunkCoordToWorld(const glm::ivec2& chunkCoord, int chunkSize, float hexRadius) {
    float hexHeight = sqrt(3.0f) * hexRadius;
    float hexWidth = 2.0f * hexRadius;
    float chunkWidth = chunkSize * (hexWidth * 0.75f);
    float chunkDepth = chunkSize * hexHeight;

    float worldX = chunkCoord.x * chunkWidth;
    float worldZ = chunkCoord.y * chunkDepth;

    return glm::vec3(worldX, 0.0f, worldZ);
}

inline glm::vec3 tileCoordToWorld(const glm::ivec3& tileCoord, const glm::ivec2& chunkCoord, int chunkSize, float hexRadius) {
    float hexHeight = sqrt(3.0f) * hexRadius;
    float hexWidth = 2.0f * hexRadius;

    glm::vec3 chunkOrigin = chunkCoordToWorld(chunkCoord, chunkSize, hexRadius);

    float xOffset = hexWidth * 0.75f * tileCoord.x;
    float zOffset = hexHeight * (tileCoord.y + 0.5f * (tileCoord.x & 1));
    float yOffset = tileCoord.z * (hexRadius / 2.0f);

    return glm::vec3(chunkOrigin.x + xOffset, yOffset, chunkOrigin.z + zOffset);
}


inline NPCInteractionElements createNPCInteractionMenu(ECS& ecs, NPCComponent& npcComponent) {
    NPCInteractionElements ids;

    // --- Create Background Panel ---
    ids.panel = ecs.createEntity();
    ecs.assignArchetype(ids.panel, uiArchetype);
    ecs.addComponent(ids.panel, UITransform({600, 400, 0}, {1200, 800}, {1.0f, 1.0f}));
    ecs.addComponent(ids.panel, UITextElement("", "default", glm::vec3(1.0f), 12.0f, false)); // Blank text
    ecs.addComponent(ids.panel, UIImageElement("npc_dialog_bg", true)); // Background visible
    ecs.addComponent(ids.panel, UIInput([](){}, [](){})); // No input behavior initially

    // --- Create NPC Portrait ---
    ids.npcPortrait = ecs.createEntity();
    ecs.assignArchetype(ids.npcPortrait, uiArchetype);
    ecs.addComponent(ids.npcPortrait, UITransform({180, 500, 0}, {200, 200}, {1.0f, 1.0f}));
    ecs.addComponent(ids.npcPortrait, UITextElement("", "default", glm::vec3(1.0f), 12.0f, false)); // Blank text
    ecs.addComponent(ids.npcPortrait, UIImageElement("npc_portrait_default", true)); // Portrait visible
    ecs.addComponent(ids.npcPortrait, UIInput([](){}, [](){})); // No input behavior

    // --- Create Header Text (NPC Name or Title) ---
    ids.headerText = ecs.createEntity();
    ecs.assignArchetype(ids.headerText, uiArchetype);
    ecs.addComponent(ids.headerText, UITransform({150, 710, 0}, {360, 40}, {1.0f, 1.0f}));
    ecs.addComponent(ids.headerText, UITextElement(npcComponent.name, "headerFont", glm::vec3(0.0f), 24.0f, true)); // Show name
    ecs.addComponent(ids.headerText, UIImageElement("", false)); // No image
    ecs.addComponent(ids.headerText, UIInput([](){}, [](){})); // No input

    ids.levelText = ecs.createEntity();
    std::string text = "Level " 
        + std::to_string(npcComponent.level)
        + " " + npcComponent.race;
    ecs.assignArchetype(ids.levelText, uiArchetype);
    ecs.addComponent(ids.levelText, UITransform({150, 690, 0}, {360, 40}, {1.0f, 1.0f}));
    ecs.addComponent(ids.levelText, UITextElement(text, "default", glm::vec3(0.0f), 24.0f, true)); // Show name
    ecs.addComponent(ids.levelText, UIImageElement("", false)); // No image
    ecs.addComponent(ids.levelText, UIInput([](){}, [](){})); // No input

    // --- Create Dialogue Text ---
    ids.npcDialogue = ecs.createEntity();
    ecs.assignArchetype(ids.npcDialogue, uiArchetype);
    ecs.addComponent(ids.npcDialogue, UITransform({150, 350, 0}, {360, 40}, {1.0f, 1.0f}));
    ecs.addComponent(ids.npcDialogue, UITextElement("Hello, Traveller.", "default", glm::vec3(0.0f), 18.0f, true)); // Example dialog
    ecs.addComponent(ids.npcDialogue, UIImageElement("", false)); // No image
    ecs.addComponent(ids.npcDialogue, UIInput([](){}, [](){})); // No input

    return ids;
}

inline InventoryMenuElements createInventoryMenu(ECS& ecs, InventoryComponent& inventoryComponent, ItemRegistry itemRegistry) {
    InventoryMenuElements ids;

    // --- Background Panel ---
    ids.panel = ecs.createEntity();
    ecs.assignArchetype(ids.panel, uiArchetype);
    ecs.addComponent(ids.panel, UITransform({600, 400, 0}, {1200, 800}, {1.0f, 1.0f}));
    ecs.addComponent(ids.panel, UITextElement("", "default", glm::vec3(1.0f), 12.0f, false));
    ecs.addComponent(ids.panel, UIImageElement("inventory_menu_bg", true));
    ecs.addComponent(ids.panel, UIInput([](){}, [](){}));

    // --- Header Text ---
    ids.headerText = ecs.createEntity();
    ecs.assignArchetype(ids.headerText, uiArchetype);
    ecs.addComponent(ids.headerText, UITransform({200, 700, 0}, {300, 50}, {1.0f, 1.0f}));
    ecs.addComponent(ids.headerText, UITextElement("Inventory & Crafting", "headerFont", glm::vec3(0.0f), 28.0f, true));
    ecs.addComponent(ids.headerText, UIImageElement("", false));
    ecs.addComponent(ids.headerText, UIInput([](){}, [](){}));

    // --- Inventory List (Dynamic based on unordered_map) ---
    float startX = 620.0f;
    float startY = 950.0f;
    float itemHeight = 40.0f;
    float spacing = 10.0f;

    size_t itemIndex = 0;
    std::cout << "populating inventory list\n";
    int i = 0;
    for (const auto& item : inventoryComponent.items) {
        std::cout << "item: " << item.itemID << "\n";
        auto itemDetails = itemRegistry.get(item.itemID);
        
        float xOffset = 48.0f + i * 80.0f; // spacing between slots
    
        ids.inventoryList.push_back(ecs.createEntity());
        ecs.assignArchetype(ids.inventoryList[i], uiArchetype);
    
        UITransform transform(
            glm::vec3(xOffset, 300.0f, -0.3f), // position
            glm::vec2(64.0f, 64.0f),           // size
            glm::vec2(1.0f, 1.0f)              // scale
        );
    
        std::cout << itemDetails.iconPath << "\n";
    
        // Optionally show quantity as text (if relevant)
        std::string displayText = item.itemID + " x" + std::to_string(item.quantity);
    
        UITextElement textElement(displayText, "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 24.0f, false);
        UIImageElement imageElement(itemDetails.iconPath, true);
        UIInput input(
            [](){},
            [](){}
        );
    
        ecs.addComponent(ids.inventoryList[i], textElement);
        ecs.addComponent(ids.inventoryList[i], imageElement);
        ecs.addComponent(ids.inventoryList[i], transform);
        ecs.addComponent(ids.inventoryList[i], input);
        i++;
    }


    // --- Crafting List (Empty for now) ---
    // You would dynamically fill this based on craftable recipes in your real system
    // Here's a static placeholder
    {
        size_t recipeEntity = ecs.createEntity();
        ecs.assignArchetype(recipeEntity, uiArchetype);

        ecs.addComponent(recipeEntity, UITransform({900.0f, 260.0f, 0}, {360, itemHeight}, {1.0f, 1.0f}));
        ecs.addComponent(recipeEntity, UITextElement("Wooden Sword", "default", glm::vec3(0.0f), 20.0f, true));
        ecs.addComponent(recipeEntity, UIImageElement("", false));
        ecs.addComponent(recipeEntity, UIInput([](){}, [](){}));

        ids.craftingList.push_back(recipeEntity);
    }

    // --- Selected Item Description ---
    ids.selectedItemDescription = ecs.createEntity();
    ecs.assignArchetype(ids.selectedItemDescription, uiArchetype);
    ecs.addComponent(ids.selectedItemDescription, UITransform({200, 410, 0}, {880, 300}, {1.0f, 1.0f}));
    ecs.addComponent(ids.selectedItemDescription, UITextElement("Select an item to view details.", "default", glm::vec3(0.0f), 18.0f, true));
    ecs.addComponent(ids.selectedItemDescription, UIImageElement("", false));
    ecs.addComponent(ids.selectedItemDescription, UIInput([](){}, [](){}));

    // --- Craft Button ---
    ids.craftButton = ecs.createEntity();
    ecs.assignArchetype(ids.craftButton, uiArchetype);
    ecs.addComponent(ids.craftButton, UITransform({1000, 300, 0}, {200, 60}, {1.0f, 1.0f}));
    ecs.addComponent(ids.craftButton, UITextElement("Craft", "default", glm::vec3(0.0f), 24.0f, true));
    ecs.addComponent(ids.craftButton, UIImageElement("textBoxTexture", true));
    ecs.addComponent(ids.craftButton, UIInput([](){
        // TODO: Implement craft button behavior
    }, [](){}));

    return ids;
}

inline void uploadLightsToShader2D(const std::vector<LightSourceComponent2D>& lights, GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    GLint numLightsLoc = glGetUniformLocation(shaderProgram, "numLights");
    glUniform1i(numLightsLoc, lights.size());

    for (size_t i = 0; i < lights.size(); ++i) {
        const auto& light = lights[i];

        std::string idx = "lights[" + std::to_string(i) + "].";

        glUniform3fv(glGetUniformLocation(shaderProgram, (idx + "position").c_str()), 1, &light.position[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, (idx + "color").c_str()), 1, &light.color[0]);
        glUniform1f(glGetUniformLocation(shaderProgram, (idx + "intensity").c_str()), light.intensity);
        glUniform1f(glGetUniformLocation(shaderProgram, (idx + "radius").c_str()), light.radius);
        glUniform1i(glGetUniformLocation(shaderProgram, (idx + "enabled").c_str()), light.enabled);
    }
}
    inline void updateInventoryBar(ECS* ecs, InventoryBarComponent& inventoryBar, InventoryComponent& inventory, ItemRegistry itemRegistry){ 
        inventoryBar.itemSlots.clear();
        inventoryBar.itemNames.clear();
        for (auto item : inventory.items){
            auto itemDetails = itemRegistry.get(item.itemID);
            inventoryBar.itemSlots.push_back(itemDetails.iconPath);
            inventoryBar.itemNames.push_back(itemDetails.name);
        }
        for (int i = 0; i < inventoryBar.itemSlots.size(); i++) {
            
            float xOffset = 48.0f + i * 80.0f; // spacing between slots
            
            size_t itemIcon = ecs -> createEntity();
            ecs -> assignArchetype(itemIcon, uiArchetype);
            
            UITransform transform(
                glm::vec3(xOffset, 750.0f, 0.0f), // position
                glm::vec2(64.0f, 64.0f),                   // size
                glm::vec2(1.0f, 1.0f)                      // scale
            );

            
            UITextElement textElement("", "Faculty-Glyphic", glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, false);
            UIImageElement imageElement(inventoryBar.itemSlots[i], true);
            UIInput input(
                [](){},
                [](){}
            );
            ecs -> addComponent(itemIcon, textElement);
            ecs -> addComponent(itemIcon, imageElement);
            ecs -> addComponent(itemIcon, transform);
            ecs -> addComponent(itemIcon, input);
        }
    }