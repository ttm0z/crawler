#version 330 core

out vec4 FragColor; // Output color

// Texture
uniform sampler2D texture1; // Texture sampler (if you plan to use textures)
in vec2 TexCoord;           // Input texture coordinate from vertex shader

void main()
{

    // Texture color
    FragColor = texture(texture1, TexCoord); // Sample texture
}
