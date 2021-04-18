#version 330 core

#include "camera.glsl"
#include "lighting.glsl"
#include "shadows_f.glsl"

///////////////////////////////////////////////////////////

in vec3 g_position;
in vec3 g_normal;
in vec3 g_frontNormal;
in vec4 g_color;
in float g_dist;

out vec4 f_color;

uniform float u_grassRadius;

const float diffFactor = 0.1f;


///////////////////////////////////////////////////////////
vec3 calcDirLightGrass(DirLight light, vec3 frontNormal, float mixFactor, vec3 viewDir, vec3 diffColor, float shadowFactor)
{
    // Get diffuse factor
    float frontDiff = dot(frontNormal, -light.direction);
    float upDiff = dot(g_normal, -light.direction);
    float diff = mix(frontDiff, upDiff, mixFactor);
    float diff1 = diffFactor * diff + diffFactor;
    float diff2 = (1.0f - diffFactor) * diff + diffFactor;
    if (diff < 0.0f)
        diff = diff1;
    else
        diff = mix(diff1, diff2, shadowFactor);
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * diffColor;

    // Get specular factor
    vec3 upReflectDir = reflect(-light.direction, g_normal);
    float upSpec = pow(max(dot(viewDir, upReflectDir), 0.0f), 20.0f);
    vec3 frontReflectDir = reflect(-light.direction, frontNormal);
    float frontSpec = pow(max(dot(viewDir, frontReflectDir), 0.0f), 20.0f);
    float spec = mix(frontSpec, upSpec, mixFactor);

    // Specular color
    vec3 specular = spec * light.specular * 0.3f * shadowFactor;

    return diffuse + specular;
}


///////////////////////////////////////////////////////////
vec3 calcPointLightGrass(PointLight light, vec3 frontNormal, float mixFactor, vec3 viewDir, vec3 diffColor)
{
    vec3 lightDir = g_position - light.position;
    float radius = length(lightDir);
    lightDir /= radius;

    // Calculate attenuation
    float attenuation = 1.0f / (light.coefficients.x + light.coefficients.y * radius + light.coefficients.z * radius * radius);

    // Get diffuse factor
    float frontDiff = dot(frontNormal, -lightDir);
    float upDiff = dot(g_normal, -lightDir);
    float diff = mix(frontDiff, upDiff, mixFactor);
    float diff1 = diffFactor * diff + diffFactor;
    float diff2 = (1.0f - diffFactor) * diff + diffFactor;
    diff = (diff < 0.0f ? diff1 : diff2);
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * diffColor;

    // Get specular factor
    vec3 upReflectDir = reflect(-lightDir, g_normal);
    float upSpec = pow(max(dot(viewDir, upReflectDir), 0.0f), 20.0f);
    vec3 frontReflectDir = reflect(-lightDir, frontNormal);
    float frontSpec = pow(max(dot(viewDir, frontReflectDir), 0.0f), 20.0f);
    float spec = mix(frontSpec, upSpec, mixFactor);

    // Specular color
    vec3 specular = spec * light.specular * 0.3f;

    return (diffuse + specular) * attenuation;
}


///////////////////////////////////////////////////////////
void main()
{
    vec3 viewDir = normalize(g_position - u_cameraPos);
    vec3 frontNormal = g_frontNormal;
    if (dot(frontNormal, -viewDir) < 0.0f)
        frontNormal = -frontNormal;

    float mixFactor = clamp(g_dist / 20.0f, 0.0f, 1.0f);
    mixFactor -= 1.0f;
    mixFactor = -0.3f * mixFactor * mixFactor + 1.0f;
        
    // Calculate lighting
    vec3 result = g_color.rgb * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
    {
        float shadowFactor = getShadowFactor(i, 0);
        result += calcDirLightGrass(u_dirLights[i], frontNormal, mixFactor, viewDir, g_color.rgb, shadowFactor);
    }
    
    // Calculate point lights
    for (int i = 0; i < u_numPointLights; ++i)
        result += calcPointLightGrass(u_pointLights[i], frontNormal, mixFactor, viewDir, g_color.rgb);

    f_color = vec4(result, g_color.a);
}