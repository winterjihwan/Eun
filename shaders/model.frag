#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Dir Light
uniform vec3 direction;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lightDir = normalize(-direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 dir_ambient = ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 dir_diffuse = diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 dir_specular = specular * spec * vec3(texture(texture_diffuse1, TexCoords));

    vec3 result = dir_ambient + dir_diffuse + dir_specular;

    FragColor = vec4(result, 1.0);
}
