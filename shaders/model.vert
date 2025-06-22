#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec2 UV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    UV = aUV;
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}
