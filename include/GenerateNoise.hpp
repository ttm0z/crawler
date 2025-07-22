#ifndef GENERATE_NOISE_H
#define GENERATE_NOISE_H

#include <string>
#include <vector>


float generateNoise(float x, float y, int octaves, float persistence);


// Function to generate land with large clusters of 1s
int generateLand(float x, float y, float threshold = 0.5f, int octaves = 5, float persistence = 0.5f);

// Example usage for a grid
void generateLandGrid(int width, int height, float threshold = 0.5f);

#endif // GENERATE_NOISE_H
