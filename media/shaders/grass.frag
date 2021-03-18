#version 330 core

#include "common.glsl"

///////////////////////////////////////////////////////////

in vec3 g_position;
in vec3 g_normal;
in vec3 g_frontNormal;
in vec4 g_color;
in float g_dist;
in vec4 g_clipSpacePos;
in vec4 g_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];

out vec4 f_color;

uniform vec3 u_cameraPos;
uniform float u_grassRadius;

uniform vec3 u_ambient;
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

uniform sampler2D u_shadowMaps[MAX_NUM_SHADOW_MAPS];
uniform float u_shadowDists[MAX_NUM_SHADOW_MAPS];
uniform float u_shadowStrengths[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadowCascades[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadows;

const float diffFactor = 0.1f;

///////////////////////////////////////////////////////////

vec3 calcDirLightGrass(DirLight light, vec3 frontNormal, float mixFactor, vec3 viewDir, vec3 diffColor, float shadowFactor)
{
    // Get diffuse factor
    float frontDiff = dot(frontNormal, -light.direction);
    float upDiff = dot(g_normal, -light.direction);
    float diff = mix(frontDiff, upDiff, mixFactor);
    float diff1 = diffFactor * diff + diffFactor;
    float diff2 = (1.0f - diffFactor) * diff + diffFactor;
    if (diff < 0.0f)
        diff = diff1;
    else
        diff = mix(diff1, diff2, shadowFactor);
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * diffColor;

    // Get specular factor
    vec3 upReflectDir = reflect(-light.direction, g_normal);
    float upSpec = pow(max(dot(viewDir, upReflectDir), 0.0f), 20.0f);
    vec3 frontReflectDir = reflect(-light.direction, frontNormal);
    float frontSpec = pow(max(dot(viewDir, frontReflectDir), 0.0f), 20.0f);
    float spec = mix(frontSpec, upSpec, mixFactor);

    // Specular color
    vec3 specular = spec * light.specular * 0.3f * shadowFactor;

    return diffuse + specular;
}

///////////////////////////////////////////////////////////

void main()
{
    vec3 viewDir = normalize(g_position - u_cameraPos);
    vec3 frontNormal = g_frontNormal;
    if (dot(frontNormal, -viewDir) < 0.0f)
        frontNormal = -frontNormal;

    float mixFactor = clamp(g_dist / 20.0f, 0.0f, 1.0f);
    mixFactor -= 1.0f;
    mixFactor = -0.3f * mixFactor * mixFactor + 1.0f;
        
    // Calculate lighting
    vec3 result = g_color.rgb * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
    {
        float shadowFactor = getShadowFactor(u_shadowMaps, g_lightClipSpacePos, u_shadowDists, u_numShadowCascades[i], g_clipSpacePos.z, i, 0);
        shadowFactor = mix(1.0f, shadowFactor, u_shadowStrengths[i]);
        result += calcDirLightGrass(u_dirLights[i], frontNormal, mixFactor, viewDir, g_color.rgb, shadowFactor);
    }

    f_color = vec4(result, g_color.a);
}