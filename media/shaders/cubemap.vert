#version 330 core

layout (location = 0) in vec3 a_position;

out vec3 v_fragPos;

uniform mat4 u_projView;

void main()
{
    gl_Position =  u_projView * vec4(a_position, 1.0);
    gl_Position = gl_Position.xyww;

    v_fragPos = a_position;
}