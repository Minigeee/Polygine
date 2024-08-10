#version 330 core

#include "../camera.glsl"

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 v_position[];
in vec2 v_size[];
in float v_rotation[];
in vec2 v_texCoord[];
in vec2 v_texSize[];
in vec4 v_color[];

out vec2 g_texCoord;
out vec4 g_color;

const float PI = 3.1415926535;


///////////////////////////////////////////////////////////
void main()
{
    // Calculate rotation stuff
    float c = cos(v_rotation[0] * PI / 180.0f);
    float s = sin(v_rotation[0] * PI / 180.0f);

    // Calculate the three local cardinal vectors of the particle
    vec3 front = normalize(u_cameraPos - v_position[0]);
    vec3 right = normalize(cross(vec3(0, 1, 0), front));
    right = normalize(right * c + cross(front, right) * s + front * dot(front, right) * (1.0f - c));
    vec3 up = normalize(cross(front, right));

    // Get corner vertices
    vec4 v1 = vec4(v_position[0] - right * v_size[0].x + up * v_size[0].y, 1.0f);
    vec4 v2 = vec4(v_position[0] - right * v_size[0].x - up * v_size[0].y, 1.0f);
    vec4 v3 = vec4(v_position[0] + right * v_size[0].x + up * v_size[0].y, 1.0f);
    vec4 v4 = vec4(v_position[0] + right * v_size[0].x - up * v_size[0].y, 1.0f);
    v1 = u_projView * v1;
    v2 = u_projView * v2;
    v3 = u_projView * v3;
    v4 = u_projView * v4;

    // Texture coords
    vec2 t1 = v_texCoord[0];
    vec2 t2 = v_texCoord[0] + vec2(v_texSize[0].x, 0.0f);
    vec2 t3 = v_texCoord[0] + vec2(0.0f, v_texSize[0].y);
    vec2 t4 = v_texCoord[0] + vec2(v_texSize[0].x, v_texSize[0].y);

    // Emit vertices
    gl_Position = v1;
    g_texCoord = t3;
    g_color = v_color[0];
    EmitVertex();
    
    gl_Position = v2;
    g_texCoord = t1;
    EmitVertex();
    
    gl_Position = v3;
    g_texCoord = t4;
    EmitVertex();
    
    gl_Position = v4;
    g_texCoord = t2;
    EmitVertex();

    EndPrimitive();
}