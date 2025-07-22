#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D imageTexture; // The texture being sampled
uniform vec4 color;             // Optional color multiplier (e.g., for tinting)

void main()
{
    vec4 texColor = texture(imageTexture, TexCoord); // Sample the texture color
    FragColor = texColor * color;                    // Apply the color multiplier
}
