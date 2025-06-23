#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;

const int MAX_BONES = 200;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_bones[MAX_BONES];

out vec3 FragPos;
out vec2 UV;
out vec3 Normal;

void main()
{
    mat4 boneTransform = u_bones[aBoneIDs.x] * aWeights.x;
    boneTransform += u_bones[aBoneIDs.y] * aWeights.y;
    boneTransform += u_bones[aBoneIDs.z] * aWeights.z;
    boneTransform += u_bones[aBoneIDs.w] * aWeights.w;

    vec4 localPos = boneTransform * vec4(aPos, 1.0);

    vec4 worldPos = u_model * localPos;
    FragPos = worldPos.xyz;
    UV = aUV;

    mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    Normal = normalMatrix * aNormal;

    gl_Position = u_projection * u_view * worldPos;
}
