#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D playerTexture; // The texture sampler

void main() {
    FragColor = texture(playerTexture, TexCoord); // Sample the texture at the given coordinates
}
