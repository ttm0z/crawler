
#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 uBaseColor; // Material base color RGBA

uniform float ambientStrength = 0.3;
uniform float specularStrength = 0.2;
uniform float shininess = 16.0;

void main()
{
    // Ambient
    vec3 ambient = ambientStrength * uBaseColor.rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uBaseColor.rgb;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * vec3(1.0); // white specular

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, uBaseColor.a);
}
