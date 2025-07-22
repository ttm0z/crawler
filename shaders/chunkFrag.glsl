#version 330 core
in vec2 TexCoord;
in float TexIndex;

uniform sampler2DArray textureArray; // Texture array sampler

out vec4 FragColor;

void main() {
    int index = int(TexIndex); // Convert float to int for indexing the texture array
    FragColor = texture(textureArray, vec3(TexCoord, index)); // Sample from texture array
}
