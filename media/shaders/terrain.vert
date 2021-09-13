#version 330 core

#include "camera.glsl"
#include "clip_planes.glsl"

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_nearTexCoord;
layout (location = 2) in vec2 a_farTexCoord;
layout (location = 3) in float a_lodDist;
layout (location = 4) in mat4 a_transform;

out vec3 v_fragPos;
out vec3 v_normal;
out vec3 v_color;
out vec2 v_texCoord;
flat out int v_chunk;

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
uniform sampler2D u_heightMaps[9];

void main()
{
    vec4 worldPos = a_transform * vec4(a_position.x, 0.0f, a_position.y, 1.0f);

    // Get chunk offset
    vec2 chunkCenter = round(u_cameraPos.xz / u_chunkSize) * u_chunkSize;
    vec2 centerOffset = worldPos.xz - chunkCenter;
    float halfChunkSize = 0.5f * u_chunkSize;

    v_chunk = 4;
    if (centerOffset.x > halfChunkSize)
        v_chunk += 3;
    else if (centerOffset.x < -halfChunkSize)
        v_chunk -= 3;
    if (centerOffset.y > halfChunkSize)
        v_chunk += 1;
    else if (centerOffset.y < -halfChunkSize)
        v_chunk -= 1;

    // Get texture coordinate
    v_texCoord = fract(centerOffset / u_chunkSize + 0.5f);

    // Calculate height
    worldPos.y = texture(u_heightMaps[v_chunk], v_texCoord).r * u_maxHeight;

    gl_Position =  u_projView * worldPos;
    
    if (u_useFlatShading)
    {
        vec3 normal, color;

        // Calculate texture coordinate
        float dist = max(abs(u_cameraPos.x - worldPos.x), abs(u_cameraPos.z - worldPos.z));

        if (dist < u_blendLodDist)
        {
            vec2 nearTexCoord = (a_transform * vec4(a_nearTexCoord.x, 0.0f, a_nearTexCoord.y, 1.0f)).xz;
            vec2 farTexCoord = (a_transform * vec4(a_farTexCoord.x, 0.0f, a_farTexCoord.y, 1.0f)).xz;

            float factor = (dist - (a_lodDist - 4.0f * u_tileScale)) / (4.0f * u_tileScale);
            factor = clamp(factor, 0.0f, 1.0f);

            vec2 texCoord = mix(nearTexCoord, farTexCoord, factor);
            texCoord = fract((texCoord - chunkCenter) / u_chunkSize + 0.5f);

            normal = texture(u_normalMaps[v_chunk], texCoord).rgb;
            color = texture(u_colorMaps[v_chunk], texCoord).rgb;
        }
        else
        {
            vec2 nearTexCoord = (a_transform * vec4(a_nearTexCoord.x, 0.0f, a_nearTexCoord.y, 1.0f)).xz;
            nearTexCoord = fract((nearTexCoord - chunkCenter) / u_chunkSize + 0.5f);

            normal = texture(u_normalMaps[v_chunk], v_texCoord).rgb * 0.3;
            normal += texture(u_normalMaps[v_chunk], nearTexCoord).rgb * 0.7;
            color = texture(u_colorMaps[v_chunk], v_texCoord).rgb;
        }

        v_normal = normal;
        v_color = color;
    }

    // Apply clip planes
    applyClipPlanes(worldPos.xyz);

    v_fragPos = worldPos.xyz;
}