#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;

const int MAX_BONES = 200;

uniform mat4 u_model;
uniform mat4 u_light_space;
uniform mat4 u_bones[MAX_BONES];

void main()
{
    mat4 boneTransform = u_bones[aBoneIDs.x] * aWeights.x;
    boneTransform += u_bones[aBoneIDs.y] * aWeights.y;
    boneTransform += u_bones[aBoneIDs.z] * aWeights.z;
    boneTransform += u_bones[aBoneIDs.w] * aWeights.w;

    vec4 localPos = boneTransform * vec4(aPos, 1.0);
    gl_Position = u_light_space * u_model * localPos;
}
