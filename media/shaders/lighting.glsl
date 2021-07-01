
#include "common.glsl"


///////////////////////////////////////////////////////////
struct DirLight
{
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};


///////////////////////////////////////////////////////////
struct PointLight
{
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    vec3 coefficients;
};


///////////////////////////////////////////////////////////
layout (std140) uniform Lights
{
    uniform vec3 u_ambient;

    uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
    uniform PointLight u_pointLights[MAX_NUM_POINT_LIGHTS];
    uniform int u_numDirLights;
    uniform int u_numPointLights;
};


///////////////////////////////////////////////////////////
vec3 calcDirLight(DirLight light, Material material, vec3 viewDir, vec3 normal, float shadowFactor, float diffFactor)
{
    // Get diffuse factor
    float diff = dot(normal, -light.direction);
    float diff1 = diffFactor * diff + diffFactor;
    float diff2 = (1.0f - diffFactor) * diff + diffFactor;
    if (diff < 0.0f)
        diff = diff1;
    else
        diff = mix(diff1, diff2, shadowFactor);
    diff = mix(1.0f, diff, material.occlusion);
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // Get specular factor
    vec3 reflectDir = reflect(-light.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // Specular color
    vec3 specular = spec * light.specular * material.specular * shadowFactor;

    return diffuse + specular;
}


///////////////////////////////////////////////////////////
vec3 calcPointLight(PointLight light, Material material, vec3 viewDir, vec3 fragPos, vec3 normal, float diffFactor)
{
    vec3 lightDir = fragPos - light.position;
    float radius = length(lightDir);
    lightDir /= radius;

    // Calculate attenuation
    float attenuation = 1.0f / (light.coefficients.x + light.coefficients.y * radius + light.coefficients.z * radius * radius);

    // Get diffuse factor
    float diff = dot(normal, -lightDir);
    float diff1 = diffFactor * diff + diffFactor;
    float diff2 = (1.0f - diffFactor) * diff + diffFactor;
    diff = (diff < 0.0f ? diff1 : diff2);
    diff = mix(1.0f, diff, material.occlusion);
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // Get specular factor
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // Specular color
    vec3 specular = spec * light.specular * material.specular;

    return (diffuse + specular) * attenuation;
}