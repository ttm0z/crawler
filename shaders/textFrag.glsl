#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text; // Glyph texture
uniform vec3 textColor; // Color of the text

void main() {
    float alpha = texture(text, TexCoords).r; // Sample red channel
    FragColor = vec4(textColor, alpha);
}
