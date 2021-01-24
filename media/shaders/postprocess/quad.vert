#version 330 core

layout (location = 0) in vec2 a_pos;

out vec2 v_texCoord;

void main()
{
    gl_Position = vec4(a_pos, 0.0f, 1.0f);
    v_texCoord = a_pos * 0.5f + 0.5f;
}