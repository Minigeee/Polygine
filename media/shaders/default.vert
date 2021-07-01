#version 330 core

#include "camera.glsl"
#include "shadows_v.glsl"

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 a_color;
layout (location = 4) in vec3 a_tangent;
layout (location = 5) in mat4 a_transform;

out vec3 v_normal;
out vec2 v_texCoord;
out vec4 v_color;
out mat3 v_tbnMatrix;

void main()
{
    vec4 worldPos = a_transform * vec4(a_position, 1.0);
    gl_Position = u_projView * worldPos;
    
    vec3 T = normalize(vec3(a_transform * vec4(a_tangent, 0.0f)));
    vec3 N = normalize(vec3(a_transform * vec4(a_normal, 0.0f)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    v_tbnMatrix = mat3(T, B, N);

    v_normal = normalize(mat3(a_transform) * a_normal);
    v_texCoord = a_texCoord;
    v_color = a_color;

    // Set up output variables for shadows
    calcShadowClipSpace(worldPos);
}