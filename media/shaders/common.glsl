
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
float getShadowFactor(sampler2D shadowMap, vec4 lightClipSpacePos, float shadowDist, float fragDist)
{
    vec3 projCoords = lightClipSpacePos.xyz / lightClipSpacePos.w;
    projCoords = projCoords * 0.5f + 0.5f;

    if (fragDist > shadowDist)
        return 1.0f;

    // Get shadow map depth
    float shadow = 0.0f;
    const int kernelHalfSize = 3;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);

    for (int r = -kernelHalfSize; r <= kernelHalfSize; ++r)
    {
        for (int c = -kernelHalfSize; c <= kernelHalfSize; ++c)
        {
            vec2 texCoords = projCoords.xy / texelSize + vec2(c, r);
            vec2 offset = random2(texCoords * 0.001f);
            texCoords += (offset - 0.5f) * 0.5f;

            float mapDepth = texture(shadowMap, texCoords * texelSize).r;
            shadow += mapDepth < projCoords.z - 0.0001f ? 1.0f : 0.0f;
        }
    }

    shadow /= 49.0f;
    
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