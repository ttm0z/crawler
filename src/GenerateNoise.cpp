#define STB_PERLIN_IMPLEMENTATION  // Only in one source file!
#include "stb_perlin.h"
#include <cmath>  // For std::pow

float generateNoise(float x, float y, int octaves = 5, float persistence = 0.4f) {
    float noiseValue = 0.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;
    float scale = 0.10f;  // Slightly larger scale

    for (int i = 0; i < octaves; ++i) {
        float frequency = scale * pow(2.0f, i);  // Frequency increases for each octave
        
        // Optionally perturb coordinates for domain warping
        float warp = 0.2f;  // Amount of warp
        float dx = stb_perlin_noise3(x * 0.05f, y * 0.05f, 0.0f, 0, 0, 0) * warp;
        float dy = stb_perlin_noise3(y * 0.05f, x * 0.05f, 0.0f, 0, 0, 0) * warp;
        
        float perlin = stb_perlin_noise3((x + dx) * frequency, (y + dy) * frequency, 0.0f, 0, 0, 0);

        noiseValue += perlin * amplitude;  // Accumulate weighted noise value
        maxValue += amplitude;

        amplitude *= persistence;  // Decay amplitude for next octave
    }

    // Normalize to [0, 1]
    float normalizedValue = (noiseValue / maxValue) * 0.5f + 0.5f;

    // Apply optional contrast boost
    normalizedValue = pow(normalizedValue, 1.3f);

    return normalizedValue;
}


// Function to generate land with large clusters of 1s
int generateLand(float x, float y, float threshold = 0.5f, int octaves = 5, float persistence = 0.5f) {
    // Generate Perlin noise value
    float noise = generateNoise(x, y, octaves, persistence);

    // Compare noise value against the threshold to determine land or water
    return (noise > threshold) ? 1 : 0;  // 1 for land, 0 for water
}

