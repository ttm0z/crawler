#version 330 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec2 aTexCoord; // Texture coordinate (if you plan to use textures)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord; // Output to fragment shader

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // Transform the vertex position
    TexCoord = aTexCoord; // Pass texture coordinates to fragment shader (if used)
}
