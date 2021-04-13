#version 330 core

#include "../noise.glsl"

layout (points) in;
layout (points, max_vertices = 10) out;

in vec3 v_position[];
in float v_rotation[];
in vec2 v_size[];
in vec4 v_color[];
in vec4 v_texRect[];
in vec3 v_velocity[];
in float v_age[];
flat in int v_type[];

out vec3 g_position;
out float g_rotation;
out vec2 g_size;
out vec4 g_color;
out vec4 g_texRect;
out vec3 g_velocity;
out float g_age;
flat out int g_type;

uniform float u_elapsed;


///////////////////////////////////////////////////////////
void main()
{
    g_position = v_position[0];
    g_rotation = v_rotation[0];
    g_size = v_size[0];
    g_color = v_color[0];
    g_texRect = v_texRect[0];
    g_velocity = v_velocity[0];
    g_age = v_age[0] + u_elapsed;
    g_type = v_type[0];
    
    if (g_type == 0)
    {
        // Create particle to maintain emitter
        EmitVertex();
        EndPrimitive();

        // Create another particle
        g_velocity.xz = rand2(vec2(g_age, 2.0f * g_age)) * 0.8f - 0.4f;
        g_age = 0.0f;
        g_type = 1;

        EmitVertex();
        EndPrimitive();
    }
    else
    {
        if (g_age > 3.0f)
            return;

        // Update position
        g_velocity.y += 9.8f * u_elapsed;
        g_position += g_velocity * u_elapsed;

        EmitVertex();
        EndPrimitive();
    }

}