#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define MAX_BONES 100
uniform mat4 boneMatrices[MAX_BONES];

void main()
{
    mat4 skinMatrix =
        aWeights.x * boneMatrices[int(aBoneIDs.x)] +
        aWeights.y * boneMatrices[int(aBoneIDs.y)] +
        aWeights.z * boneMatrices[int(aBoneIDs.z)] +
        aWeights.w * boneMatrices[int(aBoneIDs.w)];

    vec4 skinnedPos = skinMatrix * vec4(aPos, 1.0);
    vec3 skinnedNormal = mat3(skinMatrix) * aNormal;

    vec4 worldPos = model * skinnedPos;
    FragPos = worldPos.xyz;

    Normal = mat3(transpose(inverse(model))) * skinnedNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * worldPos;
}
