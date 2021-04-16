#version 330 core

#include "shadows_v.glsl"

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 v_position[];
in vec3 v_front[];
in vec3 v_right[];
in vec3 v_up[];

out vec3 g_fragPos;
out vec3 g_normal;
out vec2 g_texCoord;

uniform mat4 u_projView;

uniform vec2 u_size;
uniform vec2 u_origin;


///////////////////////////////////////////////////////////
void main()
{
    float t = (1.0f - u_origin.y) * u_size.y;
    float l = u_origin.x * u_size.x;
    float b = u_origin.y * u_size.y;
    float r = (1.0f - u_origin.x) * u_size.x;

    g_normal = v_front[0];

    // Emit vertices
    vec4 worldPos = vec4(v_position[0] + t * v_up[0] - l * v_right[0], 1.0f);
    gl_Position = u_projView * worldPos;
    g_fragPos = worldPos.xyz;
    g_texCoord = vec2(0, 1);
    calcShadowClipSpace(worldPos);
    EmitVertex();
    
    worldPos.xyz = v_position[0] - b * v_up[0] - l * v_right[0];
    gl_Position = u_projView * worldPos;
    g_fragPos = worldPos.xyz;
    g_texCoord = vec2(0, 0);
    calcShadowClipSpace(worldPos);
    EmitVertex();
    
    worldPos.xyz = v_position[0] + t * v_up[0] + r * v_right[0];
    gl_Position = u_projView * worldPos;
    g_fragPos = worldPos.xyz;
    g_texCoord = vec2(1, 1);
    calcShadowClipSpace(worldPos);
    EmitVertex();
    
    worldPos.xyz = v_position[0] - b * v_up[0] + r * v_right[0];
    gl_Position = u_projView * worldPos;
    g_fragPos = worldPos.xyz;
    g_texCoord = vec2(1, 0);
    calcShadowClipSpace(worldPos);
    EmitVertex();

    EndPrimitive();
}