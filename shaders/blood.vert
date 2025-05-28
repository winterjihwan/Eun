#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_Texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_Time;

uniform sampler2D u_PosTex;
uniform sampler2D u_NormTex;

out vec3 v_WorldNormal;
out vec3 v_ViewDir;
out vec3 v_fragPos;

void main() {
    int u_NumOfFrames = 81;
    int u_Speed = 35;
    int u_BoundingMax = 1;
    int u_BoundingMin = -1;
    vec3 u_HeightOffset = vec3(0, 0, 0);

    float currentSpeed = 1.0 / (u_NumOfFrames / float(u_Speed));
    float timeInFrames = ((ceil(fract(-u_Time * currentSpeed) * float(u_NumOfFrames))) / float(u_NumOfFrames)) + (1.0 / float(u_NumOfFrames));

    vec2 uv = a_Texcoord;

    vec4 texturePos = textureLod(u_PosTex, vec2(uv.x, timeInFrames + uv.y), 0.0);
    vec4 textureNorm = textureLod(u_NormTex, vec2(uv.x, timeInFrames + uv.y), 0.0);

    v_WorldNormal = textureNorm.xzy * 2.0 - 1.0;

    mat3 t = mat3(cross(u_model[1].xyz, u_model[2].xyz),
            cross(u_model[2].xyz, u_model[0].xyz),
            cross(u_model[0].xyz, u_model[1].xyz));

    v_WorldNormal = t * v_WorldNormal;

    gl_Position = u_projection * u_view * u_model * vec4(texturePos.xzy, 1.0);
}
