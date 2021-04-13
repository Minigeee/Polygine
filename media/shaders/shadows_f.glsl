
#include "common.glsl"
#include "noise.glsl"

// Shadow functions in the fragment shader

///////////////////////////////////////////////////////////

in vec4 v_clipSpacePos;
in vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];

uniform sampler2D u_shadowMaps[MAX_NUM_SHADOW_MAPS];
uniform float u_shadowDists[MAX_NUM_SHADOW_MAPS];
uniform float u_shadowStrengths[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadowCascades[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadows;


///////////////////////////////////////////////////////////
float getShadowFactor(int lightNum, int kernelSize)
{
    int lightIndex = lightNum * MAX_NUM_SHADOW_CASCADES;

    // Get correct light parameters
    int numCascades = u_numShadowCascades[lightIndex];
    float clipSpaceDepth = v_clipSpacePos.z;

    // Find which region the pixel is in
    int regionNum = 0;
    for (; regionNum < numCascades; ++regionNum)
    {
        if (clipSpaceDepth < u_shadowDists[lightNum * MAX_NUM_SHADOW_CASCADES + regionNum])
            break;
    }

    if (regionNum >= numCascades)
        return 1.0f;

    int mapIndex = lightNum * MAX_NUM_SHADOW_CASCADES + regionNum;
    vec3 projCoords = v_lightClipSpacePos[mapIndex].xyz / v_lightClipSpacePos[mapIndex].w;
    projCoords = projCoords * 0.5f + 0.5f;

    // Get shadow map depth
    float shadow = 0.0f;
    int kernelHalfSize = (regionNum == 0 ? kernelSize / 2 : 0);
    vec2 texelSize = 1.0f / textureSize(u_shadowMaps[mapIndex], 0);

    for (int r = -kernelHalfSize; r <= kernelHalfSize; ++r)
    {
        for (int c = -kernelHalfSize; c <= kernelHalfSize; ++c)
        {
            vec2 texCoords = projCoords.xy / texelSize + vec2(c, r) * 2.0f;
            vec2 offset = rand2(texCoords * 0.001f);
            texCoords += (offset - 0.5f);

            float mapDepth = texture(u_shadowMaps[mapIndex], texCoords * texelSize).r;
            shadow += mapDepth < projCoords.z - 0.0001f ? 1.0f : 0.0f;
        }
    }

    shadow /= pow(2 * kernelHalfSize + 1, 2.0f);
    
    return mix(1.0f, 1.0f - shadow, u_shadowStrengths[lightNum]);
}