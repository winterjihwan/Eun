#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    vec3 color = texture(skybox, TexCoords).rgb;
    color = pow(color, vec3(1.0 / 2.2));
    color *= 0.3;
    FragColor = vec4(color, 1.0);
}
