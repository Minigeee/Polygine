
layout (std140) uniform Camera
{
    mat4 u_projView;
    vec3 u_cameraPos;
    float u_near;
    float u_far;
};