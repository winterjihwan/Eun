#version 330 core

out vec4 FragColor;

in vec2 UV;

uniform sampler2D sampler;

void main()
{
    vec4 baseColor = texture(sampler, UV);

    if (baseColor.a < 0.5) {
        discard;
    }

    FragColor = vec4(0.3, 0.0, 0.0, baseColor.a);
}
