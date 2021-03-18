
#define MAX_NUM_MATERIALS 4
#define MAX_NUM_DIR_LIGHTS 2
#define MAX_NUM_SHADOW_CASCADES 3
#define MAX_NUM_SHADOW_MAPS MAX_NUM_DIR_LIGHTS * MAX_NUM_SHADOW_CASCADES


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
float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


///////////////////////////////////////////////////////////
vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}


///////////////////////////////////////////////////////////
float getShadowFactor(
    sampler2D shadowMaps[MAX_NUM_SHADOW_MAPS],
    vec4 lightClipSpacePos[MAX_NUM_SHADOW_MAPS],
    float shadowDists[MAX_NUM_SHADOW_MAPS],
    int numCascades,
    float clipSpaceDepth,
    int lightNum,
    int kernelSize)
{
    // Find which region the pixel is in
    int regionNum = 0;
    for (; regionNum < numCascades; ++regionNum)
    {
        if (clipSpaceDepth < shadowDists[lightNum * MAX_NUM_SHADOW_CASCADES + regionNum])
            break;
    }

    if (regionNum >= numCascades)
        return 1.0f;

    int mapIndex = lightNum * MAX_NUM_SHADOW_CASCADES + regionNum;
    vec3 projCoords = lightClipSpacePos[mapIndex].xyz / lightClipSpacePos[mapIndex].w;
    projCoords = projCoords * 0.5f + 0.5f;

    // Get shadow map depth
    float shadow = 0.0f;
    int kernelHalfSize = (regionNum == 0 ? kernelSize / 2 : 0);
    vec2 texelSize = 1.0f / textureSize(shadowMaps[mapIndex], 0);

    for (int r = -kernelHalfSize; r <= kernelHalfSize; ++r)
    {
        for (int c = -kernelHalfSize; c <= kernelHalfSize; ++c)
        {
            vec2 texCoords = projCoords.xy / texelSize + vec2(c, r);
            vec2 offset = random2(texCoords * 0.001f);
            texCoords += (offset - 0.5f) * 0.5f;

            float mapDepth = texture(shadowMaps[mapIndex], texCoords * texelSize).r;
            shadow += mapDepth < projCoords.z - 0.0001f ? 1.0f : 0.0f;
        }
    }

    shadow /= pow(2 * kernelHalfSize + 1, 2.0f);
    
    return 1.0f - shadow;
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