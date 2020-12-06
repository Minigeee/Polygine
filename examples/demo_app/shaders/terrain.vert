#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_nearTexCoord;
layout (location = 2) in vec2 a_farTexCoord;
layout (location = 3) in float a_lodDist;
layout (location = 4) in mat4 a_transform;

out vec3 v_fragPos;
out vec3 v_normal;
out vec4 v_color;

uniform mat4 u_projView;
uniform vec3 u_cameraPos;

uniform float u_size;
uniform float u_height;
uniform float u_resolution;
uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_colorMap;

void main()
{
    vec4 worldPos = a_transform * vec4(a_position.x, 0.0f, a_position.y, 1.0f);

    // Calculate height
    vec2 hmapTexCoord = worldPos.xz / u_size + 0.5f;
    worldPos.y = texture(u_heightMap, hmapTexCoord).r * u_height;

    gl_Position =  u_projView * worldPos;

    // Calculate texture coordinate
    vec2 nearTexCoord = (a_transform * vec4(a_nearTexCoord.x, 0.0f, a_nearTexCoord.y, 1.0f)).xz;
    vec2 farTexCoord = (a_transform * vec4(a_farTexCoord.x, 0.0f, a_farTexCoord.y, 1.0f)).xz;

    float dist = max(abs(u_cameraPos.x - nearTexCoord.x), abs(u_cameraPos.z - nearTexCoord.y));
    float factor = (dist - (a_lodDist - 4.0f * u_resolution)) / (4.0f * u_resolution);
    factor = clamp(factor, 0.0f, 1.0f);
    vec2 texCoord = mix(nearTexCoord, farTexCoord, factor);
    texCoord = texCoord / u_size + 0.5f;

    v_fragPos = worldPos.xyz;
    v_normal = texture(u_normalMap, texCoord).rgb;
    v_color = vec4(1.0f);
}