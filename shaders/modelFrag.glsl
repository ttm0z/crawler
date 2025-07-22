#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse;

#define MAX_LIGHTS 8

struct Light {
    int type; // 0 = Directional, 1 = Point, 2 = Spot

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerCutOff;
    float outerCutOff;

    bool enabled;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 albedo = texture(texture_diffuse, TexCoord).rgb;
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        if (!lights[i].enabled) continue;

        vec3 lightDir;
        float attenuation = 1.0;
        float diff = 0.0;

        if (lights[i].type == 0) {
            // Directional light
            lightDir = normalize(-lights[i].direction);
        } else {
            lightDir = normalize(lights[i].position - FragPos);

            // Attenuation
            float dist = length(lights[i].position - FragPos);
            attenuation = 1.0 / (lights[i].constant +
                                 lights[i].linear * dist +
                                 lights[i].quadratic * dist);
        }

        // Diffuse lighting
        diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].diffuse * diff;

        // Ambient lighting
        vec3 ambient = lights[i].ambient;

        // Spotlight intensity
        float spotEffect = 1.0;
        if (lights[i].type == 2) {
            float theta = dot(lightDir, normalize(-lights[i].direction));
            float epsilon = lights[i].innerCutOff - lights[i].outerCutOff;
            spotEffect = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
        }

        vec3 lighting = (ambient + diffuse * spotEffect) * attenuation;
        result += lighting;
    }

    FragColor = vec4(result * albedo, 1.0);
}
