#version 330 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D sampler;

void main()
{
    FragColor = texture(sampler, UV);
    // FragColor = vec4(0.8f, 0.5f, 0.3f, 1.0f);
}
