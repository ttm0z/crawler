#version 330 core
layout (location = 0) in vec2 aPos;      // Position of the vertex
layout (location = 1) in vec2 aTexCoord; // Texture coordinate

out vec2 TexCoord;

uniform mat4 model;       // Model matrix for transforming the image
uniform mat4 projection;   // Projection matrix (usually orthographic for 2D)

void main()
{
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0); // Calculate position
    TexCoord = aTexCoord; // Pass texture coordinate to fragment shader
}
