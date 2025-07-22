#version 330 core

in vec3 FragPos;  // Fragment position in world space
in vec3 Normal;   // Normal vector from vertex shader
in vec2 TexCoord; // Texture coordinates from vertex shader

out vec4 FragColor; // Output color

uniform sampler2D texture1; // Texture sampler

void main()
{
    // Normalize the normal
    vec3 norm = normalize(Normal);

    // For now, let's assume a simple directional light coming from above (0, 1, 0)
    vec3 lightDir = normalize(vec3(0.0, 1.0, 0.0));
    
    // Calculate diffuse lighting (simple Lambertian reflection)
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Sample the texture
    vec4 texColor = texture(texture1, TexCoord);
    
    // Combine the diffuse lighting with the texture color
    FragColor = vec4(diff * texColor.rgb, texColor.a);
}
