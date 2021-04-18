#version 330 core

#include "camera.glsl"

layout (location = 0) in mat4 a_transform;

out vec3 v_position;
out vec3 v_front;
out vec3 v_right;
out vec3 v_up;

uniform bool u_axisLocked;

void main()
{
    // Calculate billboard position
    v_position = (a_transform * vec4(0, 0, 0, 1)).xyz;

    vec3 front = normalize(u_cameraPos - v_position);
    mat3 rotScaleMatrix = mat3(a_transform);

    // Calculate right and up vectors
    vec3 tr = rotScaleMatrix * vec3(1, 0, 0);
    vec3 tu = rotScaleMatrix * vec3(0, 1, 0);
    vec3 r = normalize(cross(vec3(0, 1, 0), front));
    vec3 u = vec3(0, 1, 0);
    
    if (!u_axisLocked)
        u = normalize(cross(front, r));

    v_front = front;
    v_right = tr.x * r + tr.y * u;
    v_up = tu.x * r + tu.y * u;
}