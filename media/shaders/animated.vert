#version 330 core

#include "common.glsl"

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 a_color;
layout (location = 4) in int a_materialIndex;
layout (location = 5) in mat4 a_transform;
layout (location = 9) in vec4 a_boneWeights;
layout (location = 10) in ivec4 a_boneIds;

out vec3 v_fragPos;
out vec4 v_clipSpacePos;
out vec3 v_normal;
out vec2 v_texCoord;
out vec4 v_color;
flat out int v_materialIndex;
out vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];

uniform mat4 u_projView;
uniform mat4 u_bones[20];

uniform mat4 u_lightProjViews[MAX_NUM_SHADOW_MAPS];
uniform int u_numShadows;

void main()
{
    mat4 boneTransform = mat4(0.0f);
    for (int i = 0; i < 4; i++)
    {
        if (a_boneIds[i] >= 0)
            boneTransform += u_bones[a_boneIds[i]] * a_boneWeights[i];
    }

    vec4 worldPos = a_transform * boneTransform * vec4(a_position, 1.0);
    gl_Position =  u_projView * worldPos;
    v_clipSpacePos = gl_Position;

    v_fragPos = worldPos.xyz;
    v_normal = normalize(mat3(a_transform) * a_normal);
    v_texCoord = a_texCoord;
    v_color = a_color;
    v_materialIndex = a_materialIndex;

    // Calculate light space positions
    for (int i = 0; i < u_numShadows * MAX_NUM_SHADOW_CASCADES; ++i)
        v_lightClipSpacePos[i] = u_lightProjViews[i] * worldPos;
}