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

uniform Material u_materials;
uniform sampler2D u_diffuseMaps;
uniform sampler2D u_specularMaps;
uniform sampler2D u_normalMaps;


///////////////////////////////////////////////////////////
void main()
{
    Material material = u_materials;
    
    // Get diffuse color
    vec4 diffColor = vec4(material.diffuse, 1.0f);
    if (material.hasDiffTexture)
        diffColor *= texture(u_diffuseMaps, g_texCoord);

    vec3 result = diffColor.rgb;

    // Apply lighting if enabled
    if (u_lightingEnabled)
    {
        vec3 viewDir = normalize(g_fragPos - u_cameraPos);
        
        // Update material diffuse color
        material.diffuse = diffColor.rgb;

        // Get specular color
        if (material.hasSpecTexture)
            material.specular *= texture(u_specularMaps, g_texCoord).rgb;

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