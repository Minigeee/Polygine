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
in vec4 g_color;

out vec4 f_color;

uniform vec3 u_cameraPos;

uniform vec3 u_ambient;
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

const float diffFactor = 0.1f;

///////////////////////////////////////////////////////////

vec3 calcDirLight(DirLight light, vec3 viewDir, vec3 diffColor)
{
    // Get diffuse factor
    float diff = dot(g_normal, -light.direction);
    if (diff <= 0.0f)
        diff = diffFactor * diff + diffFactor;
    else
        diff = (1.0f - diffFactor) * diff + diffFactor;
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * diffColor;

    // Get specular factor
    vec3 reflectDir = reflect(-light.direction, g_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16.0f);

    // Specular color
    vec3 specular = spec * light.specular * 0.25f;

    return diffuse + specular;
}

///////////////////////////////////////////////////////////

void main()
{
    vec3 viewDir = normalize(g_position - u_cameraPos);
        
    // Calculate lighting
    vec3 result = g_color.rgb * u_ambient;
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
        result += calcDirLight(u_dirLights[i], viewDir, g_color.rgb);

    f_color = vec4(result, 1.0f);
}