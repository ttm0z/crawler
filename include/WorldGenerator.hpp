#pragma once
#include "vector"
#include "GenerateNoise.hpp"
#include <iostream>
#include "ECS/Components.hpp"


class WorldGenerator {
    public:
    #pragma once

        WorldGenerator(unsigned int seed);
        float euclideanDistance(int x1, int y1, int x2, int y2);
        std::vector<std::vector<ChunkData>> generateWorld(int width, int height);
        void printWorld(const std::vector<std::vector<ChunkData>>& world);
        void printElevationMap(const std::vector<std::vector<ChunkData>>& world);
        ChunkData generateChunkData(int x, int y, int width, int height);
    private:
        const int WORLD_WIDTH = 500;
        const int WORLD_HEIGHT = 500;

        const float CONTINENT_SCALE = 0.001f;
        const float ISLAND_SCALE = 0.1f;

        const float OCEAN_THRESHOLD = 0.5f;
        const float LAND_THRESHOLD = 0.5f;

        const int CHUNK_ELEVATION_RANGE = 16;
};