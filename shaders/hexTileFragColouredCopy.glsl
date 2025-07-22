#version 330 core

in vec3 TileColor;  // The base tile color passed from the vertex shader
out vec4 FragColor; // Final color output

uniform vec2 screenResolution; // The resolution of the screen (width, height)
uniform float time;            // Time passed for animation

// Function to create pseudo-random noise based on fragment position
float random(vec2 st) {
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // Fragment position in screen space (normalized to [0, 1])
    vec2 pos = gl_FragCoord.xy / screenResolution;
    
    // Create a sine-wave effect to make the pixels bounce over time (slower and less intense)
    float sineEffect = sin(pos.x * 3.0 + time * 0.5) * sin(pos.y * 3.0 + time * 0.5);
    
    // Add a smaller random noise to the effect (less pronounced)
    float randEffect = random(floor(pos * 50.0)) * 0.1;  // Reduced the random effect
    
    // Color variation (less deep)
    vec3 color = TileColor * (0.25 + 0.25 * sineEffect);  // Reduced the variation range
    
    // Add random noise to color for a subtle freaky effect
    color += randEffect;  
    
    // Make the color fluctuation slower and subtle
    color *= 1.0 + 0.05 * sin(time * 0.2 + pos.x * 3.0 + pos.y * 3.0);  // Slower fluctuation
    
    // Output the final color
    FragColor = vec4(color, 1.0);
}
