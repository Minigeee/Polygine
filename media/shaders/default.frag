#version 330 core

#include "common.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;
in vec4 v_color;
flat in int v_materialIndex;
in vec4 v_lightClipSpacePos[MAX_NUM_DIR_LIGHTS];

out vec4 f_color;

uniform vec3 u_cameraPos;

uniform vec3 u_ambient;
uniform Material u_materials[MAX_NUM_MATERIALS];
uniform sampler2D u_diffuseMaps[MAX_NUM_MATERIALS];
uniform sampler2D u_specularMaps[MAX_NUM_MATERIALS];
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

uniform sampler2D u_shadowMaps[MAX_NUM_DIR_LIGHTS];
uniform float u_shadowDists[MAX_NUM_DIR_LIGHTS];
uniform float u_shadowStrengths[MAX_NUM_DIR_LIGHTS];
uniform int u_numShadows;

///////////////////////////////////////////////////////////

void main()
{
    Material material = u_materials[v_materialIndex];
    float fragDist = distance(v_fragPos, u_cameraPos);
    vec3 viewDir = (v_fragPos - u_cameraPos) / fragDist;

    // Get diffuse color
    material.diffuse *= v_color.rgb;
    if (material.hasDiffTexture)
        material.diffuse *= texture(u_diffuseMaps[v_materialIndex], v_texCoord).rgb;

    // Get specular color
    if (material.hasSpecTexture)
        material.specular *= texture(u_specularMaps[v_materialIndex], v_texCoord).rgb;
        
    // Calculate lighting
    vec3 result = material.diffuse * material.ambient * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
    {
        float shadowFactor = getShadowFactor(u_shadowMaps[i], v_lightClipSpacePos[i], u_shadowDists[i], fragDist);
        result += calcDirLight(u_dirLights[i], material, viewDir, v_normal, shadowFactor, 0.1f);
    }

    f_color = vec4(result, 1.0f);
}