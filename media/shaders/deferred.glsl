#include "common.glsl"

layout (location = 0) out vec4 f_normalShininess;
layout (location = 1) out vec4 f_albedoOcclusion;
layout (location = 2) out vec3 f_specular;


///////////////////////////////////////////////////////////
void output(Material material, vec3 normal)
{
    f_normalShininess = vec4(normal, material.shininess);
    f_albedoOcclusion = vec4(material.diffuse, material.occlusion);
    f_specular = material.specular;
}