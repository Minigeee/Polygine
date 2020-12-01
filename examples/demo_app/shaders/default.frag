#version 330 core

///////////////////////////////////////////////////////////

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasDiffTexture;
    bool hasSpecTexture;
};

struct DirLight
{
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

///////////////////////////////////////////////////////////

#define MAX_NUM_MATERIALS 4
#define MAX_NUM_DIR_LIGHTS 2

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;
in vec4 v_color;
flat in int v_materialIndex;

out vec4 f_color;

uniform vec3 u_cameraPos;

uniform Material u_materials[MAX_NUM_MATERIALS];
uniform sampler2D u_diffuseMaps[MAX_NUM_MATERIALS];
uniform sampler2D u_specularMaps[MAX_NUM_MATERIALS];
uniform DirLight u_dirLights[MAX_NUM_DIR_LIGHTS];
uniform int u_numDirLights;

const float diffFactor = 0.1f;

///////////////////////////////////////////////////////////

vec3 calcDirLight(DirLight light, vec3 viewDir, vec3 diffColor, vec3 specColor)
{
    Material material = u_materials[v_materialIndex];

    // Get diffuse factor
    float diff = dot(v_normal, -light.direction);
    if (diff <= 0.0f)
        diff = diffFactor * diff + diffFactor;
    else
        diff = (1.0f - diffFactor) * diff + diffFactor;
        
    // Diffuse color
    vec3 diffuse = diff * light.diffuse * diffColor;

    // Get specular factor
    vec3 reflectDir = reflect(-light.direction, v_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // Specular color
    vec3 specular = spec * light.specular * specColor;

    return diffuse + specular;
}

///////////////////////////////////////////////////////////

void main()
{
    Material material = u_materials[v_materialIndex];
    vec3 viewDir = normalize(v_fragPos - u_cameraPos);

    // Get diffuse color
    vec3 diffColor = material.diffuse * v_color.rgb;
    if (material.hasDiffTexture)
        diffColor *= texture(u_diffuseMaps[v_materialIndex], v_texCoord).rgb;

    // Get specular color
    vec3 specColor = material.specular;
    if (material.hasSpecTexture)
        specColor *= texture(u_specularMaps[v_materialIndex], v_texCoord).rgb;
        
    // Calculate lighting
    vec3 result = diffColor * vec3(0.1f);
    
    // Calculate directional lighting
    for (int i = 0; i < u_numDirLights; ++i)
        result += calcDirLight(u_dirLights[i], viewDir, diffColor, specColor);

    f_color = vec4(result, 1.0f);
}