uniform mat4 finalBonesMatrices[100];
in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;
in ivec4 boneIDs;
in vec4 weights;

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
