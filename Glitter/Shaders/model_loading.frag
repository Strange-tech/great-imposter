#version 330 core

#define NUM_POINT_LIGHTS 2

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_normal1;
    sampler2D texture_specular1;
    // sampler2D emission;
    // float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
// in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirlight;
uniform PointLight pointlights[NUM_POINT_LIGHTS];

vec3 addDirectionalLight(DirectionalLight dirlight, vec3 normal, vec3 viewDir);
vec3 addPointLight(PointLight pointlight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {    
    // properties
    vec3 norm = normalize(vec3(texture(material.texture_normal1, TexCoords)));
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);

    // 1. directional light
    result += addDirectionalLight(dirlight, norm, viewDir);

    // 2. point lights
    for(int i = 0; i < NUM_POINT_LIGHTS; i++)
        result += addPointLight(pointlights[i], norm, FragPos, viewDir);

    // 3. spot light
    // result += addSpotLight(spotlight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}

vec3 addPointLight(PointLight pointlight, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pointlight.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // attenuation for point light
    float distance = length(pointlight.position - FragPos);
    float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + 
                pointlight.quadratic * (distance * distance));
    // combine
    vec3 ambient = texture(material.texture_diffuse1, TexCoords).rgb * pointlight.ambient;
    vec3 diffuse = texture(material.texture_diffuse1, TexCoords).rgb * diff * pointlight.diffuse;
    vec3 specular = texture(material.texture_specular1, TexCoords).rgb * spec * pointlight.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 addDirectionalLight(DirectionalLight dirlight, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-dirlight.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // combine results
    vec3 ambient = texture(material.texture_diffuse1, TexCoords).rgb * dirlight.ambient;
    vec3 diffuse = texture(material.texture_diffuse1, TexCoords).rgb * diff * dirlight.diffuse;
    vec3 specular = texture(material.texture_specular1, TexCoords).rgb * spec * dirlight.specular;
    return (ambient + diffuse + specular);
}