#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 UV;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    UV = aPos;
    vec4 pos = u_projection * u_view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
