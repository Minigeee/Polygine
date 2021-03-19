#version 330 core

#include "common.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec3 v_normal;
in vec3 v_color;
in vec4 v_clipSpacePos;
in vec2 v_texCoord;
in vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];

out vec4 f_color;

uniform vec3 u_cameraPos;
uniform sampler2D u_normalMap;
uniform sampler2D u_colorMap;
uniform bool u_useFlatShading;

uniform vec3 u_ambient;
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

uniform sampler2D u_shadowMaps[MAX_NUM_SHADOW_MAPS];
uniform float u_shadowDists[MAX_NUM_SHADOW_MAPS];
uniform float u_shadowStrengths[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadowCascades[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadows;

///////////////////////////////////////////////////////////

void main()
{
    float fragDist = distance(v_fragPos, u_cameraPos);
    vec3 viewDir = (v_fragPos - u_cameraPos) / fragDist;
    vec3 normal = u_useFlatShading ? v_normal : texture(u_normalMap, v_texCoord).rgb;
    vec3 color = u_useFlatShading ? v_color : texture(u_colorMap, v_texCoord).rgb;

    // Create terrain material
    Material material;
    material.diffuse = color;
    material.specular = vec3(0.2f);
    material.shininess = 20.0f;
        
    // Calculate lighting
    vec3 result = color * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
    {
        float shadowFactor = getShadowFactor(u_shadowMaps, v_lightClipSpacePos, u_shadowDists, u_numShadowCascades[i], v_clipSpacePos.z, i, 3);
        shadowFactor = mix(1.0f, shadowFactor, u_shadowStrengths[i]);
        result += calcDirLight(u_dirLights[i], material, viewDir, normal, shadowFactor, 0.1f);
    }

    f_color = vec4(result, 1.0f);
}