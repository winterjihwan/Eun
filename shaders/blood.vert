#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform float u_Time;

uniform sampler2D u_PosTex;
uniform sampler2D u_NormTex;

out vec3 v_WorldNormal;

void main() {
    int numOfFrames = 81;
    int speed = 35;

    float currentSpeed = 1.0 / (numOfFrames / float(speed));
    float timeInFrames = ((ceil(fract(u_Time * currentSpeed) * float(numOfFrames))) / float(numOfFrames)) + (1.0 / float(numOfFrames));

    vec2 uv = tex;

    vec4 texturePos = textureLod(u_PosTex, vec2(uv.x, timeInFrames + uv.y), 0.0);
    vec4 textureNorm = textureLod(u_NormTex, vec2(uv.x, timeInFrames + uv.y), 0.0);

    v_WorldNormal = textureNorm.xzy * 2.0 - 1.0;

    mat3 t = mat3(cross(u_Model[1].xyz, u_Model[2].xyz),
            cross(u_Model[2].xyz, u_Model[0].xyz),
            cross(u_Model[0].xyz, u_Model[1].xyz));

    v_WorldNormal = t * v_WorldNormal;

    gl_Position = u_Projection * u_View * u_Model * vec4(texturePos.xzy, 1.0);
}
