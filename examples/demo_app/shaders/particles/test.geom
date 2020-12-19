#version 330 core

layout (points) in;
layout (points, max_vertices = 10) out;

flat in int v_type[];
in vec3 v_position[];
in vec2 v_size[];
in float v_rotation[];
in vec2 v_texCoord[];
in vec2 v_texSize[];
in vec4 v_color[];
in vec3 v_velocity[];
in float v_angularVelocity[];
in float v_elapsed[];

flat out int g_type;
out vec3 g_position;
out vec2 g_size;
out float g_rotation;
out vec2 g_texCoord;
out vec2 g_texSize;
out vec4 g_color;
out vec3 g_velocity;
out float g_angularVelocity;
out float g_elapsed;

uniform float u_elapsed;


///////////////////////////////////////////////////////////
void main()
{
    vec3 velocity = v_velocity[0];
    vec3 pos = v_position[0];

    if (v_type[0] == 1 && v_elapsed[0] < 5.0f)
    {
        velocity += vec3(0, 9.8f, 0) * u_elapsed;
        pos += velocity * u_elapsed;
    }

    g_type = v_type[0];
    g_position = pos;
    g_size = v_size[0];
    g_rotation = v_rotation[0];
    g_texCoord = v_texCoord[0];
    g_texSize = v_texSize[0];
    g_color = v_color[0];
    g_velocity = velocity;
    g_angularVelocity = v_angularVelocity[0];
    g_elapsed = v_elapsed[0] + u_elapsed;

    if (v_elapsed[0] < 5.0f)
    {
        EmitVertex();
        EndPrimitive();
    }

    if (v_type[0] == 0)
    {
        g_type = 1;
        g_position = v_position[0];
        g_size = vec2(0.05f);
        g_rotation = 0.0f;
        g_color = vec4(1,0,0,1);
        g_velocity = vec3(0);
        g_angularVelocity = 0.0f;
        g_elapsed = 0.0f;
        EmitVertex();
        EndPrimitive();
    }
}