#version 330 core

#include "camera.glsl"
#include "shadows_v.glsl"

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 a_color;
layout (location = 6) in mat4 a_transform;

out vec3 v_fragPos;
out vec3 v_normal;
out vec2 v_texCoord;
out vec4 v_color;

void main()
{
    vec4 worldPos = a_transform * vec4(a_position, 1.0);
    gl_Position = u_projView * worldPos;

    v_fragPos = worldPos.xyz;
    v_normal = normalize(mat3(a_transform) * a_normal);
    v_texCoord = a_texCoord;
    v_color = a_color;

    // Set up output variables for shadows
    calcShadowClipSpace(worldPos);
}