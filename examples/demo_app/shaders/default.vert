#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 a_color;

uniform mat4 m_projView;
uniform mat4 m_transform;

void main()
{
    gl_Position =  m_projView * vec4(a_position, 1.0);
}