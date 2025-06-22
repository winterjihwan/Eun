#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;

const int MAX_BONES = 200;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];

out vec2 TexCoords;

void main()
{
    mat4 BoneTransform = bones[aBoneIDs.x] * aWeights.x;
    BoneTransform += bones[aBoneIDs.y] * aWeights.y;
    BoneTransform += bones[aBoneIDs.z] * aWeights.z;
    BoneTransform += bones[aBoneIDs.w] * aWeights.w;

    vec4 localPos = BoneTransform * vec4(aPosition, 1.0);
    gl_Position = projection * view * model * localPos;
    TexCoords = aTexCoord;
}
