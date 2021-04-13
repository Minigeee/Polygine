
#include "common.glsl"


///////////////////////////////////////////////////////////
struct DirLight
{
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
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
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // Get specular factor
    vec3 reflectDir = reflect(-light.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // Specular color
    vec3 specular = spec * light.specular * material.specular * shadowFactor;

    return diffuse + specular;
}