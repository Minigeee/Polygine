#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in float a_rotation;
layout (location = 2) in vec2 a_size;
layout (location = 3) in vec4 a_color;
layout (location = 4) in vec4 a_texRect;
layout (location = 5) in vec3 a_velocity;
layout (location = 6) in float a_age;
layout (location = 7) in int a_type;

out vec3 v_position;
out float v_rotation;
out vec2 v_size;
out vec4 v_color;
out vec4 v_texRect;
out vec3 v_velocity;
out float v_age;
flat out int v_type;


///////////////////////////////////////////////////////////
void main()
{
    v_position = a_position;
    v_rotation = a_rotation;
    v_size = a_size;
    v_color = a_color;
    v_texRect = a_texRect;
    v_velocity = a_velocity;
    v_age = a_age;
    v_type = a_type;
}