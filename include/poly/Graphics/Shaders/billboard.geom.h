#ifndef SHADER_BILLBOARD_GEOM
#define SHADER_BILLBOARD_GEOM "#version 330 core\n\nlayout (std140) uniform Camera\n{\n    mat4 u_projView;\n    vec3 u_cameraPos;\n    float u_near;\n    float u_far;\n};\n#define MAX_NUM_MATERIALS 4\n#define MAX_NUM_DIR_LIGHTS 2\n#define MAX_NUM_SHADOW_CASCADES 3\n#define MAX_NUM_SHADOW_MAPS MAX_NUM_DIR_LIGHTS * MAX_NUM_SHADOW_CASCADES\n#define MAX_NUM_POINT_LIGHTS 100\n\n\n///////////////////////////////////////////////////////////\nstruct Material\n{\n    vec3 diffuse;\n    vec3 specular;\n    float shininess;\n    float occlusion;\n    float reflectivity;\n    bool hasDiffTexture;\n    bool hasSpecTexture;\n    bool hasNormalTexture;\n};\nlayout (std140) uniform Shadows\n{\n    uniform mat4 u_lightProjViews[MAX_NUM_SHADOW_MAPS];\n    uniform float u_shadowDists[MAX_NUM_SHADOW_MAPS];\n    uniform float u_shadowStrengths[MAX_NUM_DIR_LIGHTS];\n    uniform int u_numShadowCascades[MAX_NUM_DIR_LIGHTS];\n    uniform bool u_shadowsEnabled[MAX_NUM_DIR_LIGHTS];\n};\n\n// Set up shadows in the vertex shader\n\n///////////////////////////////////////////////////////////\n\n#ifndef DEFERRED_SHADING\nout vec4 v_clipSpacePos;\nout vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];\n#else\nvec4 v_clipSpacePos;\nvec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];\n#endif\n\n\n///////////////////////////////////////////////////////////\nvoid calcShadowClipSpace(vec4 worldPos)\n{\n    #ifndef DEFERRED_SHADING\n    v_clipSpacePos = gl_Position;\n    #else\n    v_clipSpacePos = u_projView * worldPos;\n    #endif\n\n    // Calculate light space positions\n    for (int i = 0; i < MAX_NUM_DIR_LIGHTS; ++i)\n    {\n        if (u_shadowsEnabled[i])\n        {\n            int start = i * MAX_NUM_SHADOW_CASCADES;\n            int end = start + MAX_NUM_SHADOW_CASCADES;\n\n            for (int j = start; j < end; ++j)\n                v_lightClipSpacePos[j] = u_lightProjViews[j] * worldPos;\n        }\n    }\n}\n\nlayout (points) in;\nlayout (triangle_strip, max_vertices = 4) out;\n\nin vec3 v_position[];\nin vec3 v_front[];\nin vec3 v_right[];\nin vec3 v_up[];\n\nout vec3 g_fragPos;\nout vec3 g_normal;\nout vec2 g_texCoord;\n\nuniform vec2 u_size;\nuniform vec2 u_origin;\n\n\n///////////////////////////////////////////////////////////\nvoid main()\n{\n    float t = (1.0f - u_origin.y) * u_size.y;\n    float l = u_origin.x * u_size.x;\n    float b = u_origin.y * u_size.y;\n    float r = (1.0f - u_origin.x) * u_size.x;\n\n    g_normal = v_front[0];\n\n    // Emit vertices\n    vec4 worldPos = vec4(v_position[0] + t * v_up[0] - l * v_right[0], 1.0f);\n    gl_Position = u_projView * worldPos;\n    g_fragPos = worldPos.xyz;\n    g_texCoord = vec2(0, 1);\n    calcShadowClipSpace(worldPos);\n    EmitVertex();\n    \n    worldPos.xyz = v_position[0] - b * v_up[0] - l * v_right[0];\n    gl_Position = u_projView * worldPos;\n    g_fragPos = worldPos.xyz;\n    g_texCoord = vec2(0, 0);\n    calcShadowClipSpace(worldPos);\n    EmitVertex();\n    \n    worldPos.xyz = v_position[0] + t * v_up[0] + r * v_right[0];\n    gl_Position = u_projView * worldPos;\n    g_fragPos = worldPos.xyz;\n    g_texCoord = vec2(1, 1);\n    calcShadowClipSpace(worldPos);\n    EmitVertex();\n    \n    worldPos.xyz = v_position[0] - b * v_up[0] + r * v_right[0];\n    gl_Position = u_projView * worldPos;\n    g_fragPos = worldPos.xyz;\n    g_texCoord = vec2(1, 0);\n    calcShadowClipSpace(worldPos);\n    EmitVertex();\n\n    EndPrimitive();\n}"
#endif