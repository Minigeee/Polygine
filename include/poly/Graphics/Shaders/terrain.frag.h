#ifndef SHADER_TERRAIN_FRAG
#define SHADER_TERRAIN_FRAG "#version 330 core\n\nlayout (std140) uniform Camera\n{\n    mat4 u_projView;\n    vec3 u_cameraPos;\n    float u_near;\n    float u_far;\n};\n#define MAX_NUM_MATERIALS 4\n#define MAX_NUM_DIR_LIGHTS 2\n#define MAX_NUM_SHADOW_CASCADES 3\n#define MAX_NUM_SHADOW_MAPS MAX_NUM_DIR_LIGHTS * MAX_NUM_SHADOW_CASCADES\n#define MAX_NUM_POINT_LIGHTS 100\n\n\n///////////////////////////////////////////////////////////\nstruct Material\n{\n    vec3 diffuse;\n    vec3 specular;\n    float shininess;\n    float occlusion;\n    bool hasDiffTexture;\n    bool hasSpecTexture;\n    bool hasNormalTexture;\n};\n\nlayout (location = 0) out vec4 f_normalShininess;\nlayout (location = 1) out vec4 f_albedoOcclusion;\nlayout (location = 2) out vec3 f_specular;\n\n\n///////////////////////////////////////////////////////////\nvoid output(Material material, vec3 normal)\n{\n    f_normalShininess = vec4(normal, material.shininess);\n    f_albedoOcclusion = vec4(material.diffuse, material.occlusion);\n    f_specular = material.specular;\n}\n\n///////////////////////////////////////////////////////////\n\nin vec3 v_fragPos;\nin vec3 v_normal;\nin vec3 v_color;\nin vec2 v_texCoord;\n\nlayout (std140) uniform Terrain\n{\n    uniform vec4 u_clipPlanes[4];\n\n    uniform float u_size;\n    uniform float u_height;\n    uniform float u_tileScale;\n    uniform float u_blendLodDist;\n    uniform bool u_useFlatShading;\n};\n\nuniform sampler2D u_normalMap;\nuniform sampler2D u_colorMap;\n\n///////////////////////////////////////////////////////////\n\nvoid main()\n{\n    vec3 normal = u_useFlatShading ? v_normal : texture(u_normalMap, v_texCoord).rgb;\n    vec3 color = u_useFlatShading ? v_color : texture(u_colorMap, v_texCoord).rgb;\n\n    // Create terrain material\n    Material material;\n    material.diffuse = color;\n    material.specular = vec3(0.2f);\n    material.shininess = 20.0f;\n    material.occlusion = 1.0f;\n    \n    // Output to color buffers\n    output(material, normal);\n}"
#endif