#version 330 core

#include "camera.glsl"
#include "deferred.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec3 v_normal;
in vec3 v_color;
in vec2 v_texCoord;
flat in int v_chunk;

layout (std140) uniform Terrain
{
    uniform float u_chunkSize;
    uniform float u_maxHeight;
    uniform float u_tileScale;
    uniform float u_blendLodDist;
    uniform bool u_useFlatShading;
};

uniform sampler2D u_normalMaps[9];
uniform sampler2D u_colorMaps[9];

///////////////////////////////////////////////////////////

void main()
{
    vec3 normal = u_useFlatShading ? v_normal : texture(u_normalMaps[v_chunk], v_texCoord).rgb;
    vec3 color = u_useFlatShading ? v_color : texture(u_colorMaps[v_chunk], v_texCoord).rgb;

    // Create terrain material
    Material material;
    material.diffuse = color;
    material.specular = vec3(0.2f);
    material.shininess = 20.0f;
    material.occlusion = 1.0f;
    material.reflectivity = 0.0f;
    
    // Output to color buffers
    deferred(material, normal);
}