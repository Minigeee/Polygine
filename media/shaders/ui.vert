#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in float a_rotation;
layout (location = 2) in vec2 a_size;
layout (location = 3) in vec2 a_origin;
layout (location = 4) in vec4 a_color;
layout (location = 5) in vec4 a_texRect;
layout (location = 6) in float a_index;

out vec2 v_position;
out float v_rotation;
out vec2 v_size;
out vec2 v_origin;
out vec4 v_color;
out vec2 v_texCoord;
out vec2 v_texSize;
out float v_index;


///////////////////////////////////////////////////////////
void main()
{
    v_position = a_position;
    v_size = a_size;
    v_rotation = a_rotation;
    v_origin = a_origin;
    v_texCoord = a_texRect.xy;
    v_texSize = a_texRect.zw;
    v_color = a_color;
    v_index = a_index;
}