#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 a_color;
layout (location = 4) in int a_materialIndex;

out vec2 v_texCoord;
out vec4 v_color;
flat out int v_materialIndex;

uniform mat4 u_projView;
uniform mat4 u_transform;

void main()
{
    gl_Position =  u_projView * vec4(a_position, 1.0);

    v_texCoord = a_texCoord;
    v_color = a_color;
    v_materialIndex = a_materialIndex;
}