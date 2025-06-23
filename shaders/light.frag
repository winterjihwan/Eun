#version 330 core

out vec4 FragColor;

in vec2 UV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct PointLight {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct FlashLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool is_on;
};

const int NR_LIGHTS = 32;
uniform PointLight pointLights[NR_LIGHTS];
uniform DirLight dirLight;
uniform FlashLight flashLight;
uniform vec3 viewPos;
uniform float shininess;

vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 albedo, float specular);
vec3 CalcFlashLight(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 albedo, float specular);

void main()
{
    vec3 FragPos = texture(gPosition, UV).rgb;
    vec3 Normal = normalize(texture(gNormal, UV).rgb);
    vec3 Albedo = texture(gAlbedoSpec, UV).rgb;
    float Specular = texture(gAlbedoSpec, UV).a;
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional Light
    vec3 lighting = CalcDirLight(Normal, viewDir, Albedo, Specular);

    // Point lights
    for (int i = 0; i < NR_LIGHTS; ++i)
    {
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = diff * Albedo * pointLights[i].color;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), shininess);
        vec3 specular = pointLights[i].color * spec * Specular;

        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);

        lighting += attenuation * (diffuse + specular);
    }

    // Flashlight
    if (flashLight.is_on) {
        lighting += CalcFlashLight(FragPos, Normal, viewDir, Albedo, Specular);
    }

    FragColor = vec4(lighting, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 albedo, float specular)
{
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = dirLight.ambient * albedo;
    vec3 diffuse = dirLight.diffuse * diff * albedo;
    vec3 specularColor = dirLight.specular * spec * specular;

    return ambient + diffuse + specularColor;
}

vec3 CalcFlashLight(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 albedo, float specular)
{
    vec3 lightDir = normalize(flashLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(flashLight.position - fragPos);
    float attenuation = 1.0 / (flashLight.constant + flashLight.linear * distance + flashLight.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-flashLight.direction));
    float epsilon = flashLight.cutOff - flashLight.outerCutOff;
    float intensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = flashLight.ambient * albedo;
    vec3 diffuse = flashLight.diffuse * diff * albedo;
    vec3 specularColor = flashLight.specular * spec * specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specularColor *= attenuation * intensity;

    return ambient + diffuse + specularColor;
}
