#version 330 core

#define NR_POINT_LIGHTS 2

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirlight;
uniform PointLight pointlights[NR_POINT_LIGHTS];
uniform SpotLight spotlight;

vec3 addDirectionalLight(DirectionalLight dirlight, vec3 normal, vec3 viewDir);
vec3 addPointLight(PointLight pointlight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 addSpotLight(SpotLight spotlight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);

    // 1. directional light
    result += addDirectionalLight(dirlight, norm, viewDir);

    // 2. point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += addPointLight(pointlights[i], norm, FragPos, viewDir);

    // 3. spot light
    result += addSpotLight(spotlight, norm, FragPos, viewDir);

    // optional, add emission light
    // vec3 emission = vec3(0.0f);
    // if(texture(material.specular, TexCoords).rgb == vec3(0.0f)) {
    //     emission = texture(material.emission, TexCoords).rgb * 0.5f;
    // }

    FragColor = vec4(result, 1.0f);
}

vec3 addDirectionalLight(DirectionalLight dirlight, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-dirlight.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * dirlight.ambient;
    vec3 diffuse = texture(material.diffuse, TexCoords).rgb * diff * dirlight.diffuse;
    vec3 specular = texture(material.specular, TexCoords).rgb * spec * dirlight.specular;
    return (ambient + diffuse + specular);
}

vec3 addPointLight(PointLight pointlight, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pointlight.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation for point light
    float distance = length(pointlight.position - FragPos);
    float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + 
                pointlight.quadratic * (distance * distance));
    // combine
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * pointlight.ambient;
    vec3 diffuse = texture(material.diffuse, TexCoords).rgb * diff * pointlight.diffuse;
    vec3 specular = texture(material.specular, TexCoords).rgb * spec * pointlight.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 addSpotLight(SpotLight spotlight, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(spotlight.position - fragPos);
    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutoff - spotlight.outer_cutoff;
    float intensity = clamp((theta - spotlight.outer_cutoff) / epsilon, 0.0, 1.0);

    if(theta > spotlight.outer_cutoff) {
        // ambient
        vec3 ambient = texture(material.diffuse, TexCoords).rgb * spotlight.ambient;

        // diffuse
        vec3 lightDir = normalize(spotlight.position - FragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = texture(material.diffuse, TexCoords).rgb * diff * spotlight.diffuse;

        // specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = texture(material.specular, TexCoords).rgb * spec * spotlight.specular;

        diffuse  *= intensity;
        specular *= intensity;
        return (ambient + diffuse + specular);
    }
    // else, only ambient
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * spotlight.ambient;
    return ambient;
}















