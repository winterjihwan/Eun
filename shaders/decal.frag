#version 330 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec2 UV;
in vec3 Normal;

uniform sampler2D sampler;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = texture(sampler, UV).rgb;
    gAlbedoSpec.a = 1.0f;
}
