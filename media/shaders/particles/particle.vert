#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in float a_rotation;
layout (location = 2) in vec2 a_size;
layout (location = 3) in vec4 a_color;
layout (location = 4) in vec4 a_texRect;

out vec3 v_position;
out vec2 v_size;
out float v_rotation;
out vec2 v_texCoord;
out vec2 v_texSize;
out vec4 v_color;


///////////////////////////////////////////////////////////
void main()
{
    v_position = a_position;
    v_rotation = a_rotation;
    v_size = a_size;
    v_color = a_color;
    v_texCoord = a_texRect.xy;
    v_texSize = a_texRect.zw;
}