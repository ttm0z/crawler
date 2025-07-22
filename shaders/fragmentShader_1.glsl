#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Define material struct
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;  // Declare the uniform for the material

uniform vec3 lightPos;   // Light position in world space
uniform vec3 viewPos;    // Camera position
uniform vec3 lightColor; // Light color

// Texture sampler
uniform sampler2D texture1;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * material.ambient * lightColor;  // Use material.ambient
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * lightColor;  // Use material.diffuse
    
    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);  // Use material.shininess
    vec3 specular = specularStrength * spec * material.specular * lightColor;  // Use material.specular

    // Texture sampling
    vec4 texColor = texture(texture1, TexCoords); // Sample the texture
    vec3 result = (ambient + diffuse + specular) * texColor.rgb; // Combine texture color with lighting results
    FragColor = vec4(result, 1.0);
}
