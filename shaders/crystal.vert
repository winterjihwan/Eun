#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

void main()
{
    vec4 worldPos = u_model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    Normal = normalize(normalMatrix * aNormal);

    UV = aUV;
    gl_Position = u_projection * u_view * worldPos;
}
