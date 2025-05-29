#version 410 core

out vec4 FragColor;

in vec3 v_WorldNormal;

uniform float u_Time;

void main()
{
    vec3 bloodColor = vec3(0.4, 0, 0);

    FragColor = vec4(bloodColor, 1.0);
}
