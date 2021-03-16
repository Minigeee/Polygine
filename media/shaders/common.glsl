
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
float getShadowFactor(sampler2D shadowMap, vec4 lightClipSpacePos, float shadowDist, float fragDist)
{
    vec3 projCoords = lightClipSpacePos.xyz / lightClipSpacePos.w;
    projCoords = projCoords * 0.5f + 0.5f;

    if (fragDist > shadowDist)
        return 1.0f;

    // Get shadow map depth
    float mapDepth = texture(shadowMap, projCoords.xy).r;
    
    return mapDepth < projCoords.z - 0.0001f ? 0.0f : 1.0f;
}


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