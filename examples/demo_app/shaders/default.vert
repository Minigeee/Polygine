#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 transform;

out vec3 Color;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    Color = vec3(aPos.x + 0.5, 1.0, aPos.y + 0.5);
}