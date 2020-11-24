#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 a_color;
layout (location = 4) in int a_materialIndex;
layout (location = 5) in vec4 a_boneWeights;
layout (location = 6) in ivec4 a_boneIds;

out vec2 v_texCoord;
out vec4 v_color;
flat out int v_materialIndex;

uniform mat4 u_projView;
uniform mat4 u_transform;
uniform mat4 u_bones[20];

void main()
{
    mat4 boneTransform = mat4(0.0f);
    for (int i = 0; i < 4; i++)
    {
        if (a_boneIds[i] >= 0)
            boneTransform += u_bones[a_boneIds[i]] * a_boneWeights[i];
    }

    vec4 pos = u_transform * boneTransform * vec4(a_position, 1.0);
    gl_Position =  u_projView * pos;

    v_texCoord = a_texCoord;
    v_color = a_color;
    v_materialIndex = a_materialIndex;
}