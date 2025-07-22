#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexIndex; // Texture index for the array

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;
out float TexIndex; // Pass texture index to fragment shader

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
    TexIndex = aTexIndex; // Pass texture index
}
