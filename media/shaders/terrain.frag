#version 330 core

#include "camera.glsl"
#include "deferred.glsl"

///////////////////////////////////////////////////////////

in vec3 v_fragPos;
in vec2 v_texCoord;

uniform sampler2D u_normalMap;

///////////////////////////////////////////////////////////

void main()
{
    vec3 normal = texture(u_normalMap, v_texCoord).xyz;
    vec3 color = vec3(0.4f, 0.8f, 0.4f);

    // Create terrain material
    Material material;
    material.diffuse = color;
    material.specular = vec3(0.2f);
    material.shininess = 2.0f;
    material.occlusion = 1.0f;
    material.reflectivity = 0.0f;
    
    // Output to color buffers
    deferred(material, normal);
}