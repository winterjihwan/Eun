#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 weights;

uniform mat4 finalBonesMatrices[100];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    mat4 boneTransform =
        weights.x * finalBonesMatrices[boneIDs.x] +
            weights.y * finalBonesMatrices[boneIDs.y] +
            weights.z * finalBonesMatrices[boneIDs.z] +
            weights.w * finalBonesMatrices[boneIDs.w];

    vec4 worldPosition = model * boneTransform * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPosition;

    FragPos = vec3(worldPosition);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}
