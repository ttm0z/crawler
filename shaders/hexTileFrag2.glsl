#version 330 core
out vec4 FragColor;

uniform float u_time;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    vec2 st = gl_FragCoord.xy / vec2(800.0, 600.0);
    float r = random(st + u_time); // Add time for animation
    float g = random(st * 1.3 + u_time);
    float b = random(st * 2.1 + u_time);

    FragColor = vec4(r, g, b, 1.0);
}
