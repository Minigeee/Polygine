#version 330 core

#include "common.glsl"

in vec2 g_texCoord;

out vec4 f_color;

uniform Material u_materials[1];
uniform sampler2D u_diffuseMaps[1];
uniform sampler2D u_specularMaps[1];
uniform sampler2D u_normalMaps[1];


///////////////////////////////////////////////////////////
void main()
{
    Material material = u_materials[0];
    f_color.a = 1.0f;
    
    // Get diffuse color
    f_color.rgb = material.diffuse;
    if (material.hasDiffTexture)
        f_color *= texture(u_diffuseMaps[0], g_texCoord);
}