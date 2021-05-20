#version 330 core

#include "camera.glsl"

layout (location = 0) in vec2 a_position;
layout (location = 1) in float a_spacing;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_color;
out float v_density;
out float v_spacing;
out float v_grassWidth;
out float v_grassHeight;

uniform float u_terrainSize;
uniform float u_terrainHeight;
uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_colorMap;
uniform sampler2D u_densityMap;
uniform sampler2D u_sizeMap;
uniform bool u_useColorMap;
uniform bool u_useDensityMap;
uniform bool u_useSizeMap;

uniform vec3 u_grassColor;
uniform float u_grassWidth;
uniform float u_grassHeight;

///////////////////////////////////////////////////////////

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

///////////////////////////////////////////////////////////

void main()
{
    // Set x and z coords
    v_position.x = round((a_position.x + u_cameraPos.x) / a_spacing) * a_spacing;
    v_position.z = round((a_position.y + u_cameraPos.z) / a_spacing) * a_spacing;

    // Offset by random value
    vec2 offset = vec2(rand(v_position.xz), rand(v_position.zx)) - 0.5f;
    v_position.xz += offset * a_spacing;

    // Get terrain texture coordinates
    vec2 texCoord = v_position.xz / u_terrainSize + 0.5f;

    // Get terrain height
    v_position.y = texture(u_heightMap, texCoord).r * u_terrainHeight;

    // Get normal
    v_normal = texture(u_normalMap, texCoord).rgb;

    // Terrain color
    if (u_useColorMap)
        v_color = texture(u_colorMap, texCoord).rgb;
    else
        v_color = u_grassColor;

    // Get density value from map
    if (u_useDensityMap)
        v_density = texture(u_densityMap, texCoord).r;
    else
        v_density = 1.0f;

    // Grass size
    if (u_useSizeMap)
    {
        vec2 size = texture(u_sizeMap, texCoord).rg;
        v_grassWidth = size.x;
        v_grassHeight = size.y;
    }
    else
    {
        v_grassWidth = u_grassWidth;
        v_grassHeight = u_grassHeight;
    }

    // Set base a_spacing
    v_spacing = a_spacing;
}