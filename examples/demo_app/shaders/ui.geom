#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 v_position[];
in float v_rotation[];
in vec2 v_size[];
in vec2 v_origin[];
in vec2 v_texCoord[];
in vec2 v_texSize[];
in vec4 v_color[];
in float v_index[];

out vec2 g_texCoord;
out vec4 g_color;

uniform vec2 u_targetSize;

const float PI = 3.1415926535;


///////////////////////////////////////////////////////////
void main()
{
    // Calculate rotation stuff
    float c = cos(-v_rotation[0] * PI / 180.0f);
    float s = sin(-v_rotation[0] * PI / 180.0f);

    vec2 right = vec2(c, s);
    vec2 up = vec2(-s, c);
    float l_scale = v_origin[0].x;
    float r_scale = 1.0f - v_origin[0].x;
    float t_scale = v_origin[0].y;
    float b_scale = 1.0f - v_origin[0].y;

    // Get corner vertices
    vec4 v1 = vec4(v_position[0] - right * v_size[0].x * l_scale - up * v_size[0].y * t_scale, v_index[0], 1.0f);
    vec4 v2 = vec4(v_position[0] - right * v_size[0].x * l_scale + up * v_size[0].y * b_scale, v_index[0], 1.0f);
    vec4 v3 = vec4(v_position[0] + right * v_size[0].x * r_scale - up * v_size[0].y * t_scale, v_index[0], 1.0f);
    vec4 v4 = vec4(v_position[0] + right * v_size[0].x * r_scale + up * v_size[0].y * b_scale, v_index[0], 1.0f);
    v1.xy /= u_targetSize;
    v2.xy /= u_targetSize;
    v3.xy /= u_targetSize;
    v4.xy /= u_targetSize;
    v1.y = 1.0f - v1.y;
    v2.y = 1.0f - v2.y;
    v3.y = 1.0f - v3.y;
    v4.y = 1.0f - v4.y;
    v1.xy = v1.xy * 2.0f - 1.0f;
    v2.xy = v2.xy * 2.0f - 1.0f;
    v3.xy = v3.xy * 2.0f - 1.0f;
    v4.xy = v4.xy * 2.0f - 1.0f;

    // Texture coords
    vec2 t1 = v_texCoord[0];
    vec2 t2 = v_texCoord[0] + vec2(v_texSize[0].x, 0.0f);
    vec2 t3 = v_texCoord[0] + vec2(0.0f, v_texSize[0].y);
    vec2 t4 = v_texCoord[0] + vec2(v_texSize[0].x, v_texSize[0].y);

    // Emit vertices
    gl_Position = v1;
    g_texCoord = t1;
    g_color = v_color[0];
    EmitVertex();
    
    gl_Position = v2;
    g_texCoord = t2;
    EmitVertex();
    
    gl_Position = v3;
    g_texCoord = t3;
    EmitVertex();
    
    gl_Position = v4;
    g_texCoord = t4;
    EmitVertex();

    EndPrimitive();
}