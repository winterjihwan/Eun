#version 410 core

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

in vec4 Color;
in vec2 TexCoord;

void main() {
    FragColor = texture(texture_diffuse1, TexCoord);
    FragColor.rgba *= Color;
}
