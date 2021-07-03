#version 330 core

#include "procedural_skybox.glsl"

in vec3 v_fragPos;

out vec4 f_color;


///////////////////////////////////////////////////////////
void main()
{
    // Get the view direction
    vec3 viewDir = normalize(v_fragPos);

    // Final color
    f_color = vec4(getSkyColor(viewDir), 1.0f);
}