#version 410 core

in vec2 vTexCoords;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, vTexCoords).r);
    FragColor = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
}
