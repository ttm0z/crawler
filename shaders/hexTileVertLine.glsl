#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec3 fragColor;

void main() {
    fragColor = aColor;
    gl_Position = projection * view * model * vec4(aPosition, 0.0, 1.0);
}
