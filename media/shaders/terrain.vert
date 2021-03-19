#version 330 core

#include "common.glsl"

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_nearTexCoord;
layout (location = 2) in vec2 a_farTexCoord;
layout (location = 3) in float a_lodDist;
layout (location = 4) in mat4 a_transform;

out vec3 v_fragPos;
out vec3 v_normal;
out vec3 v_color;
out vec4 v_clipSpacePos;
out vec2 v_texCoord;
out vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];

uniform mat4 u_projView;
uniform vec3 u_cameraPos;
uniform vec4 u_clipPlanes[4];

uniform sampler2D u_normalMap;
uniform sampler2D u_colorMap;

uniform float u_size;
uniform float u_height;
uniform float u_tileScale;
uniform float u_blendLodDist;
uniform sampler2D u_heightMap;
uniform bool u_useFlatShading;

uniform mat4 u_lightProjViews[MAX_NUM_SHADOW_MAPS];
uniform int u_numShadows;

float gl_ClipDistance[4];

void main()
{
    vec4 worldPos = a_transform * vec4(a_position.x, 0.0f, a_position.y, 1.0f);

    // Calculate height
    v_texCoord = worldPos.xz / u_size + 0.5f;
    worldPos.y = texture(u_heightMap, v_texCoord).r * u_height;

    gl_Position =  u_projView * worldPos;
    v_clipSpacePos = gl_Position;
    
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
            texCoord = texCoord / u_size + 0.5f;

            normal = texture(u_normalMap, texCoord).rgb;
            color = texture(u_colorMap, texCoord).rgb;
        }
        else
        {
            vec2 nearTexCoord = (a_transform * vec4(a_nearTexCoord.x, 0.0f, a_nearTexCoord.y, 1.0f)).xz;

            normal = texture(u_normalMap, worldPos.xz / u_size + 0.5f).rgb * 0.3;
            normal += texture(u_normalMap, nearTexCoord / u_size + 0.5f).rgb * 0.7;
            color = texture(u_colorMap, worldPos.xz / u_size + 0.5f).rgb;
        }

        v_normal = normal;
        v_color = color;
    }

    // Apply clip planes
    for (int i = 0; i < 4; ++i)
        gl_ClipDistance[i] = dot(worldPos, u_clipPlanes[i]);

    v_fragPos = worldPos.xyz;

    // Calculate light space positions
    for (int i = 0; i < u_numShadows * MAX_NUM_SHADOW_CASCADES; ++i)
        v_lightClipSpacePos[i] = u_lightProjViews[i] * worldPos;
}