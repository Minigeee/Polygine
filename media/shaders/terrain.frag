#version 330 core

#include "camera.glsl"
#include "deferred.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec3 v_normal;
in vec3 v_color;
flat in int v_chunk;

layout (std140) uniform Terrain
{
    uniform float u_chunkSize;
    uniform float u_maxHeight;
    uniform float u_tileScale;
    uniform float u_blendLodDist;
    uniform bool u_useFlatShading;
};

uniform Material u_material;
uniform sampler2D u_normalMaps[9];
uniform sampler2D u_colorMaps[9];

///////////////////////////////////////////////////////////

void main()
{
    vec2 chunkCenter = round(v_fragPos.xz / u_chunkSize) * u_chunkSize;
    vec2 texCoord = clamp((v_fragPos.xz - chunkCenter) / u_chunkSize + 0.5f, 0.0f, 1.0f);

    vec2 mapSize = textureSize(u_normalMaps[v_chunk], 0);
    vec2 ntexCoord = (mapSize - 1.0f) / mapSize * texCoord + 0.5f / mapSize;
    vec3 normal = u_useFlatShading ? v_normal : texture(u_normalMaps[v_chunk], ntexCoord).rgb;

    mapSize = textureSize(u_colorMaps[v_chunk], 0);
    vec2 ctexCoord = (mapSize - 1.0f) / mapSize * texCoord + 0.5f / mapSize;
    vec3 color = u_useFlatShading ? v_color : texture(u_colorMaps[v_chunk], ctexCoord).rgb;

    // Create terrain material
    Material material;
    material.diffuse = color;
    material.specular = u_material.specular;
    material.shininess = u_material.shininess;
    material.occlusion = u_material.occlusion;
    material.reflectivity = u_material.reflectivity;
    
    // Output to color buffers
    deferred(material, normal);
}