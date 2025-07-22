#version 330 core
in vec3 TileColor;    // Tile color from the vertex shader
out vec4 FragColor;         // Final color output

void main() {
    FragColor = vec4(TileColor, 1.0);
}