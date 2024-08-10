#version 330 core

#include "camera.glsl"
#include "terrain_map.glsl"

layout (location = 0) in vec2 a_vertex;
layout (location = 1) in vec4 a_instance;

out vec3 v_fragPos;
out vec2 v_texCoord;

uniform float u_size;
uniform float u_maxHeight;
uniform float u_lodRanges[20];

uniform sampler2D u_heightMap;


void main()
{
    // Calculate position
    vec3 localPos = vec3(a_vertex.x, 0.0f, a_vertex.y) * a_instance.z;
    vec4 worldPos = vec4(localPos + vec3(a_instance.x, 0.0f, a_instance.y), 1.0f);

    // Get height
    v_texCoord = worldPos.xz / u_size + 0.5f;
    worldPos.y = sampleBase(u_heightMap, v_texCoord).x * u_maxHeight;

    // Geomorph
    int lod = int(a_instance.w);
    float sRange = u_lodRanges[lod + 1];
    float eRange = u_lodRanges[lod];
    float morphStart = mix(sRange, eRange, 0.8f);

    float d = distance(u_cameraPos, worldPos.xyz);
    float morphFactor = clamp((d - morphStart) / (eRange - morphStart), 0.0f, 1.0f);
    vec2 isOdd = fract(a_vertex * 0.5f) * 2.0f;
    worldPos.xz -= isOdd * a_instance.z * morphFactor;

    // Resample height if morph changed position
    v_texCoord = worldPos.xz / u_size + 0.5f;
    worldPos.y = sample(u_heightMap, v_texCoord).x * u_maxHeight;

    // Clip space pos
    gl_Position = u_projView * worldPos;

    v_fragPos = worldPos.xyz;
}