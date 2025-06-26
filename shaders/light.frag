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

uniform DirLight u_dirLight;
uniform FlashLight u_flashLight;
uniform vec3 u_viewPos;
uniform float u_shininess;
uniform vec3 u_lightPos;
uniform mat4 u_light_space;

float CalcShadow(vec3 fragPosWorldSpace, vec3 normal);
vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 albedo, float specular);
vec3 CalcFlashLight(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 albedo, float specular);

void main()
{
    vec3 fragPos = texture(gPosition, UV).rgb;
    vec3 normal = normalize(texture(gNormal, UV).rgb);
    vec3 albedo = texture(gAlbedoSpec, UV).rgb;
    float specular = texture(gAlbedoSpec, UV).a;
    vec3 viewDir = normalize(u_viewPos - fragPos);

    // Shadow
    float shadow = CalcShadow(fragPos, normal);

    // Directional Light
    vec3 lighting = CalcDirLight(normal, viewDir, albedo, specular);

    // Flashlight
    if (u_flashLight.is_on) {
        lighting += CalcFlashLight(fragPos, normal, viewDir, albedo, specular);
    }

    lighting *= (1.0 - shadow / 2); // shadow / 2 is HACK
    FragColor = vec4(lighting, 1.0);
}

float CalcShadow(vec3 fragPosWorldSpace, vec3 normal)
{
    // Light space position
    vec4 fragPosLightSpace = u_light_space * vec4(fragPosWorldSpace, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Depth values
    float currentDepth = projCoords.z;
    float closestDepth = texture(gShadow, projCoords.xy).r;

    // Bias
    vec3 lightDir = normalize(u_lightPos - fragPosWorldSpace);
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
    vec3 lightDir = normalize(-u_dirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);

    vec3 ambient = u_dirLight.ambient * albedo;
    vec3 diffuse = u_dirLight.diffuse * diff * albedo;
    vec3 specularColor = u_dirLight.specular * spec * specular;

    return ambient + diffuse + specularColor;
}

vec3 CalcFlashLight(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 albedo, float specular)
{
    vec3 lightDir = normalize(u_flashLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);

    float distance = length(u_flashLight.position - fragPos);
    float attenuation = 1.0 / (u_flashLight.constant + u_flashLight.linear * distance + u_flashLight.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-u_flashLight.direction));
    float epsilon = u_flashLight.cutOff - u_flashLight.outerCutOff;
    float intensity = clamp((theta - u_flashLight.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = u_flashLight.ambient * albedo;
    vec3 diffuse = u_flashLight.diffuse * diff * albedo;
    vec3 specularColor = u_flashLight.specular * spec * specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specularColor *= attenuation * intensity;

    return ambient + diffuse + specularColor;
}
