#version 330 core

layout (location = 0) in vec3 aPos;   // Vertex position
layout (location = 1) in vec3 aNormal; // Vertex color
layout (location = 2) in vec2 aTexCoord; // Vertex color

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;      // Model matrix
uniform mat4 view;       // View matrix
uniform mat4 projection; // Projection matrix

void main()
{
    // Calculate the vertex position in world space
    fragPos = vec3(model * vec4(aPos, 1.0));
    
    // Pass normal to the fragment shader (transform normal by the inverse-transpose of the model matrix)
    normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass texture coordinates to the fragment shader
    texCoord = aTexCoord;

    // Compute final position of the vertex
    gl_Position = projection * view * vec4(fragPos, 1.0);
}
