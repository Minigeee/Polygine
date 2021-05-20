
#define MAX_NUM_MATERIALS 4
#define MAX_NUM_DIR_LIGHTS 2
#define MAX_NUM_SHADOW_CASCADES 3
#define MAX_NUM_SHADOW_MAPS MAX_NUM_DIR_LIGHTS * MAX_NUM_SHADOW_CASCADES
#define MAX_NUM_POINT_LIGHTS 32


///////////////////////////////////////////////////////////
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasDiffTexture;
    bool hasSpecTexture;
    bool hasNormalTexture;
};