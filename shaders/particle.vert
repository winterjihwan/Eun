#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aOffset;
layout(location = 2) in float aSize;
layout(location = 3) in float aAlpha;
layout(location = 4) in float aFrame;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec3 u_camera_right;
uniform vec3 u_camera_up;

out vec2 vUV;
out float vAlpha;
flat out float vFrame;

void main() {
    vec3 worldPos = aOffset + (aPos.x * aSize) * u_camera_right + (aPos.y * aSize) * u_camera_up;
    gl_Position = u_projection * u_view * vec4(worldPos, 1.0);

    vUV = aPos * 0.5 + 0.5;
    vAlpha = aAlpha;
    vFrame = aFrame;
}
