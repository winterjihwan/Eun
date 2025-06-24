#version 330 core

out vec4 FragColor;

in vec2 UV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gShadow;

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

uniform DirLight dirLight;
uniform FlashLight flashLight;
uniform vec3 viewPos;
uniform float shininess;
uniform vec3 lightPos;
uniform mat4 lightSpaceMatrix;

float CalcShadow(vec3 fragPosWorldSpace, vec3 normal, vec3 lightPos);
vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 albedo, float specular);
vec3 CalcFlashLight(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 albedo, float specular);

void main()
{
    vec3 FragPos = texture(gPosition, UV).rgb;
    vec3 Normal = normalize(texture(gNormal, UV).rgb);
    vec3 Albedo = texture(gAlbedoSpec, UV).rgb;
    float Specular = texture(gAlbedoSpec, UV).a;
    vec3 viewDir = normalize(viewPos - FragPos);

    // Shadow
    float shadow = CalcShadow(FragPos, Normal, lightPos);

    // Directional Light
    vec3 lighting = CalcDirLight(Normal, viewDir, Albedo, Specular);

    // Flashlight
    if (flashLight.is_on) {
        lighting += CalcFlashLight(FragPos, Normal, viewDir, Albedo, Specular);
    }

    lighting *= (1.0 - shadow);
    FragColor = vec4(lighting, 1.0);
}

float CalcShadow(vec3 fragPosWorldSpace, vec3 normal, vec3 lightPos)
{
    // Light space position
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPosWorldSpace, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Depth values
    float currentDepth = projCoords.z;
    float closestDepth = texture(gShadow, projCoords.xy).r;

    // Bias
    vec3 lightDir = normalize(lightPos - fragPosWorldSpace);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(gShadow, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(gShadow, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Cull Light frustum
    if (projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
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
