#version 330 core

#include "camera.glsl"
#include "lighting.glsl"
#include "shadows_f.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;
in vec4 v_color;
in mat3 v_tbnMatrix;

out vec4 f_color;

uniform Material u_material;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_normalMap;

///////////////////////////////////////////////////////////

void main()
{
    Material material = u_material;
    float fragDist = distance(v_fragPos, u_cameraPos);
    vec3 viewDir = (v_fragPos - u_cameraPos) / fragDist;

    // Get diffuse color
    material.diffuse *= v_color.rgb;
    if (material.hasDiffTexture)
        material.diffuse *= texture(u_diffuseMap, v_texCoord).rgb;

    // Get specular color
    if (material.hasSpecTexture)
        material.specular *= texture(u_specularMap, v_texCoord).rgb;

    vec3 normal = v_normal;
    if (material.hasNormalTexture)
        normal = normalize(v_tbnMatrix * (texture(u_normalMap, v_texCoord).rgb * 2.0f - 1.0f));
        
    // Calculate lighting
    vec3 result = material.diffuse * material.ambient * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
    {
        float shadowFactor = getShadowFactor(i, normal, 3);
        result += calcDirLight(u_dirLights[i], material, viewDir, normal, shadowFactor, 0.1f);
    }
    
    // Calculate point lights
    for (int i = 0; i < u_numPointLights; ++i)
        result += calcPointLight(u_pointLights[i], material, viewDir, v_fragPos, normal, 0.1f);

    f_color = vec4(result, 1.0f);
}