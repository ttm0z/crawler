#version 330 core

layout(location = 0) in vec3 aPos; // Position of the vertex

uniform mat4 projection; // Projection matrix
uniform mat4 view;       // View matrix

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
}
