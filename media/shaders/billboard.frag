#version 330 core

#include "deferred.glsl"

///////////////////////////////////////////////////////////

in vec3 g_normal;
in vec2 g_texCoord;

uniform bool u_lightingEnabled;

uniform Material u_materials;
uniform sampler2D u_diffuseMaps;
uniform sampler2D u_specularMaps;
uniform sampler2D u_normalMaps;


///////////////////////////////////////////////////////////
void main()
{
    Material material = u_materials;

    // Get diffuse color
    material.diffuse *= v_color.rgb;
    if (material.hasDiffTexture)
        material.diffuse *= texture(u_diffuseMap, v_texCoord).rgb;

    // Get specular color
    if (material.hasSpecTexture)
        material.specular *= texture(u_specularMap, v_texCoord).rgb;
        
    deferred(material, u_lightingEnabled ? g_normal : vec3(0.0f));
}