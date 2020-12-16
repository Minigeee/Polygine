#version 330 core

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

uniform vec3 u_cameraPos;
uniform mat4 u_projView;

const float PI = 3.1415926535;


///////////////////////////////////////////////////////////
void main()
{
    // Calculate rotation stuff
    float c = cos(v_rotation[0] * PI / 180.0f);
    float s = sin(v_rotation[0] * PI / 180.0f);

    // Calculate the three local cardinal vectors of the particle
    vec3 front = normalize(v_position - u_cameraPos);
    vec3 right = cross(front, vec3(0, 1, 0));
    right = right * c + cross(front, right) * s + front * dot(front, right) * (1.0f - c);
    vec3 up = cross(right, front);

    // Get corner vertices
    vec4 v1 = vec4(v_position - right * v_size[0].x + up * v_size[0].y, 1.0f);
    vec4 v2 = vec4(v_position - right * v_size[0].x - up * v_size[0].y, 1.0f);
    vec4 v3 = vec4(v_position + right * v_size[0].x + up * v_size[0].y, 1.0f);
    vec4 v4 = vec4(v_position + right * v_size[0].x - up * v_size[0].y, 1.0f);
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