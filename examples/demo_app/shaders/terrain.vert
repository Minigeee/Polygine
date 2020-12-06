#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_nearTexCoord;
layout (location = 2) in vec2 a_farTexCoord;
layout (location = 3) in mat4 a_transform;

out vec3 v_fragPos;
out vec3 v_normal;
out vec4 v_color;

uniform mat4 u_projView;
uniform float u_size;
uniform float u_height;
uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;

void main()
{
    vec4 worldPos = a_transform * vec4(a_position.x, 0.0f, a_position.y, 1.0f);

    // Calculate height
    vec2 hmapTexCoord = worldPos.xz / u_size + 0.5f;
    worldPos.y = texture(u_heightMap, hmapTexCoord).r * u_height;

    gl_Position =  u_projView * worldPos;

    // Calculate texture coordinate
    vec2 texCoord = (a_transform * vec4(a_nearTexCoord.x, 0.0f, a_nearTexCoord.y, 1.0f)).xz;
    texCoord = texCoord / u_size + 0.5f;

    v_fragPos = worldPos.xyz;
    v_normal = texture(u_normalMap, texCoord).rgb;
    v_color = vec4(1.0f);
}