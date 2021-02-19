#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_nearTexCoord;
layout (location = 2) in vec2 a_farTexCoord;
layout (location = 3) in float a_lodDist;
layout (location = 4) in mat4 a_transform;

out vec3 v_fragPos;
out vec2 v_texCoord;

uniform mat4 u_projView;
uniform vec3 u_cameraPos;
uniform vec4 u_clipPlanes[4];

uniform float u_size;
uniform float u_height;
uniform float u_tileScale;
uniform float u_blendLodDist;
uniform sampler2D u_heightMap;

float gl_ClipDistance[4];

void main()
{
    vec4 worldPos = a_transform * vec4(a_position.x, 0.0f, a_position.y, 1.0f);

    // Calculate height
    v_texCoord = worldPos.xz / u_size + 0.5f;
    worldPos.y = texture(u_heightMap, v_texCoord).r * u_height;

    gl_Position =  u_projView * worldPos;

    // Apply clip planes
    for (int i = 0; i < 4; ++i)
        gl_ClipDistance[i] = dot(worldPos, u_clipPlanes[i]);

    v_fragPos = worldPos.xyz;
}