#version 330 core

#include "camera.glsl"
#include "common.glsl"
#include "lighting.glsl"
#include "shadows_f.glsl"

in vec3 g_fragPos;
in vec3 g_normal;
in vec2 g_texCoord;

out vec4 f_color;

uniform bool u_lightingEnabled;
uniform bool u_shadowingEnabled;

uniform vec3 u_ambient;
uniform Material u_materials[1];
uniform sampler2D u_diffuseMaps[1];
uniform sampler2D u_specularMaps[1];
uniform sampler2D u_normalMaps[1];

uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;


///////////////////////////////////////////////////////////
void main()
{
    Material material = u_materials[0];
    
    // Get diffuse color
    vec4 diffColor = vec4(material.diffuse, 1.0f);
    if (material.hasDiffTexture)
        diffColor *= texture(u_diffuseMaps[0], g_texCoord);

    vec3 result = diffColor.rgb;

    // Apply lighting if enabled
    if (u_lightingEnabled)
    {
        vec3 viewDir = normalize(g_fragPos - u_cameraPos);
        
        // Update material diffuse color
        material.diffuse = diffColor.rgb;

        // Get specular color
        if (material.hasSpecTexture)
            material.specular *= texture(u_specularMaps[0], g_texCoord).rgb;

        result *= material.ambient * u_ambient;
        
        // Calculate directional lighting
        for (int i = 0; i < u_numDirLights; ++i)
        {
            float shadowFactor = 1.0f;
            if (u_shadowingEnabled)
                shadowFactor = getShadowFactor(i, 3);

            result += calcDirLight(u_dirLights[i], material, viewDir, g_normal, shadowFactor, 0.1f);
        }
    }

    f_color = vec4(result, diffColor.a);
}