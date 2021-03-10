
#define MAX_NUM_MATERIALS 4
#define MAX_NUM_DIR_LIGHTS 2


///////////////////////////////////////////////////////////
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasDiffTexture;
    bool hasSpecTexture;
};


///////////////////////////////////////////////////////////
struct DirLight
{
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};


///////////////////////////////////////////////////////////
vec3 calcDirLight(DirLight light, Material material, vec3 viewDir, vec3 normal, float shadowFactor = 1.0f, float diffFactor = 0.1f)
{
    // Get diffuse factor
    float diff = dot(normal, -light.direction);
    if (diff <= 0.0f)
        diff = diffFactor * diff + diffFactor;
    else
        diff = (1.0f - diffFactor) * diff + diffFactor;
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * material.diffuse * shadowFactor;

    // Get specular factor
    vec3 reflectDir = reflect(-light.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // Specular color
    vec3 specular = spec * light.specular * material.specular;

    return diffuse + specular;
}