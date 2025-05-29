#version 330 core

out vec4 FragColor;

in vec2 vUV;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 baseColor = texture(texture_diffuse1, vUV);

    if (baseColor.a < 0.5) {
        discard;
    }

    FragColor = vec4(0.4, 0.0, 0.0, baseColor.a);
}
