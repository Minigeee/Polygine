#version 330 core

#define DEFERRED_SHADING

#include "camera.glsl"
#include "lighting.glsl"
#include "shadows_v.glsl"
#include "shadows_f.glsl"


///////////////////////////////////////////////////////////
in vec2 v_texCoord;

out vec4 f_color;

uniform mat4 u_invProjView;

uniform sampler2D u_normalShininess;
uniform sampler2D u_albedoOcclusion;
uniform sampler2D u_specular;
uniform sampler2D u_depth;


///////////////////////////////////////////////////////////////////////////////
vec3 getFragPos(vec2 uv, out float depth)
{
    depth = 2.0f * texture(u_depth, uv).r - 1.0f;
    vec4 clipPos = vec4(2.0f * uv - 1.0f, depth, 1.0f);
    vec4 pos = u_invProjView * clipPos;

    return pos.xyz / pos.w;
}


///////////////////////////////////////////////////////////
void main()
{
    vec4 normalShininess = texture(u_normalShininess, v_texCoord);
    vec3 normal = normalShininess.xyz;
    float depth = 0.0f;
    vec3 position = getFragPos(v_texCoord, depth);

    // Create the material
    vec4 albedoOcclusion = texture(u_albedoOcclusion, v_texCoord);

    Material material;
    material.diffuse = albedoOcclusion.rgb;
    material.specular = texture(u_specular, v_texCoord).rgb;
    material.shininess = normalShininess.w;
    material.occlusion = albedoOcclusion.a;


    // Calculate view direction
    float fragDist = distance(position, u_cameraPos);
    vec3 viewDir = (position - u_cameraPos) / fragDist;

    // Calculate shadow variables
    calcShadowClipSpace(vec4(position, 1.0f));
        
    // Calculate lighting
    vec3 result = material.diffuse * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
    {
        float shadowFactor = getShadowFactor(i, normal, 3);
        result += calcDirLight(u_dirLights[i], material, viewDir, normal, shadowFactor, 0.1f);
    }
    
    // Calculate point lights
    for (int i = 0; i < u_numPointLights; ++i)
        result += calcPointLight(u_pointLights[i], material, viewDir, position, normal, 0.1f);

    f_color = vec4(result, 1.0f);
}