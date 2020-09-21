#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 m_projView;
uniform mat4 m_transform;

void main()
{
    gl_Position =  m_projView * vec4(aPos, 1.0);
}