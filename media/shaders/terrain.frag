#version 330 core

#include "common.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec2 v_texCoord;

out vec4 f_color;

uniform vec3 u_cameraPos;
uniform sampler2D u_normalMap;
uniform sampler2D u_colorMap;

uniform vec3 u_ambient;
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

///////////////////////////////////////////////////////////

void main()
{
    vec3 viewDir = normalize(v_fragPos - u_cameraPos);
    vec3 normal = texture(u_normalMap, v_texCoord).rgb;
    vec3 color = texture(u_colorMap, v_texCoord).rgb;

    // Create terrain material
    Material material;
    material.diffuse = color;
    material.specular = vec3(0.2f);
    material.shininess = 20.0f;
        
    // Calculate lighting
    vec3 result = color * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
        result += calcDirLight(u_dirLights[i], material, viewDir, normal);

    f_color = vec4(result, 1.0f);
}