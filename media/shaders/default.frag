#version 330 core

#include "deferred.glsl"

///////////////////////////////////////////////////////////

in vec3 v_normal;
in vec2 v_texCoord;
in vec4 v_color;
in mat3 v_tbnMatrix;

uniform Material u_material;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_normalMap;

///////////////////////////////////////////////////////////

void main()
{
    Material material = u_material;

    // Get diffuse color
    material.diffuse *= v_color.rgb;
    if (material.hasDiffTexture)
        material.diffuse *= texture(u_diffuseMap, v_texCoord).rgb;

    // Get specular color
    if (material.hasSpecTexture)
        material.specular *= texture(u_specularMap, v_texCoord).rgb;

    // Get normal
    vec3 normal = v_normal;
    if (material.hasNormalTexture)
        normal = normalize(v_tbnMatrix * (texture(u_normalMap, v_texCoord).rgb * 2.0f - 1.0f));
        
    deferred(material, normal);
}