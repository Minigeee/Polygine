#version 330 core

in vec3 v_fragPos;

out vec4 f_color;

uniform samplerCube u_cubemap;


///////////////////////////////////////////////////////////
void main()
{
    // Final color
    f_color.rgb = texture(u_cubemap, v_fragPos).rgb;
}