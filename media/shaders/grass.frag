#version 330 core

///////////////////////////////////////////////////////////

struct DirLight
{
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

///////////////////////////////////////////////////////////

#define MAX_NUM_DIR_LIGHTS 2

in vec3 g_position;
in vec3 g_normal;
in vec3 g_frontNormal;
in vec4 g_color;
in float g_dist;

out vec4 f_color;

uniform vec3 u_cameraPos;
uniform float u_grassRadius;

uniform vec3 u_ambient;
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

const float diffFactor = 0.1f;

///////////////////////////////////////////////////////////

vec3 calcDirLight(DirLight light, vec3 frontNormal, float mixFactor, vec3 viewDir, vec3 diffColor)
{
    // Get diffuse factor
    float frontDiff = dot(frontNormal, -light.direction);
    float upDiff = dot(g_normal, -light.direction);
    float diff = mix(frontDiff, upDiff, mixFactor);
    if (diff <= 0.0f)
        diff = diffFactor * diff + diffFactor;
    else
        diff = (1.0f - diffFactor) * diff + diffFactor;
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * diffColor;

    // Get specular factor
    vec3 upReflectDir = reflect(-light.direction, g_normal);
    float upSpec = pow(max(dot(viewDir, upReflectDir), 0.0f), 20.0f);
    vec3 frontReflectDir = reflect(-light.direction, frontNormal);
    float frontSpec = pow(max(dot(viewDir, frontReflectDir), 0.0f), 20.0f);
    float spec = mix(frontSpec, upSpec, mixFactor);

    // Specular color
    vec3 specular = spec * light.specular * 0.3f;

    return diffuse + specular;
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
    mixFactor = -0.25f * mixFactor * mixFactor + 1.0f;
        
    // Calculate lighting
    vec3 result = g_color.rgb * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
        result += calcDirLight(u_dirLights[i], frontNormal, mixFactor, viewDir, g_color.rgb);

    f_color = vec4(result, g_color.a);
}