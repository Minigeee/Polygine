#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_nearTexCoord;
layout (location = 2) in vec2 a_farTexCoord;
layout (location = 3) in mat4 a_transform;

out vec3 v_fragPos;
out vec3 v_normal;
out vec4 v_color;

uniform mat4 u_projView;

void main()
{
    vec4 worldPos = a_transform * vec4(a_position.x, 0.0f, a_position.y, 1.0f);
    gl_Position =  u_projView * worldPos;

    v_fragPos = worldPos.xyz;
    v_normal = normalize(mat3(a_transform) * vec3(0.0f, 1.0f, 0.0f));
    v_color = vec4(normalize(vec3(abs(a_nearTexCoord), 0.0f)), 1.0f);
}