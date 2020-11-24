#version 330 core

///////////////////////////////////////////////////////////

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

///////////////////////////////////////////////////////////

#define MAX_NUM_MATERIALS 4

in vec2 v_texCoord;
in vec4 v_color;
flat in int v_materialIndex;

out vec4 f_color;

uniform Material u_materials[MAX_NUM_MATERIALS];
uniform sampler2D u_diffuseMaps[MAX_NUM_MATERIALS];
uniform sampler2D u_specularMaps[MAX_NUM_MATERIALS];

///////////////////////////////////////////////////////////

void main()
{
    Material material = u_materials[v_materialIndex];

    vec3 diffuse = texture(u_diffuseMaps[v_materialIndex], v_texCoord).rgb * material.diffuse;
    f_color = vec4(diffuse, 1.0f) * v_color;
}