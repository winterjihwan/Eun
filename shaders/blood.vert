#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_time;

uniform sampler2D u_posTex;
uniform sampler2D u_normTex;

out vec3 WorldNormal;

void main() {
    int numOfFrames = 81;
    int speed = 35;

    float currentSpeed = 1.0 / (numOfFrames / float(speed));
    float timeInFrames = ((ceil(fract(u_time * currentSpeed) * float(numOfFrames))) / float(numOfFrames)) + (1.0 / float(numOfFrames));

    vec2 uv = aUV;

    vec4 texturePos = textureLod(u_posTex, vec2(uv.x, timeInFrames + uv.y), 0.0);
    vec4 textureNorm = textureLod(u_normTex, vec2(uv.x, timeInFrames + uv.y), 0.0);

    WorldNormal = textureNorm.xzy * 2.0 - 1.0;

    mat3 t = mat3(cross(u_model[1].xyz, u_model[2].xyz),
            cross(u_model[2].xyz, u_model[0].xyz),
            cross(u_model[0].xyz, u_model[1].xyz));

    WorldNormal = t * WorldNormal;

    gl_Position = u_projection * u_view * u_model * vec4(texturePos.xzy, 1.0);
}
