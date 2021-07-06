#ifndef SHADER_POSTPROCESS_REFLECTIONS_FRAG
#define SHADER_POSTPROCESS_REFLECTIONS_FRAG "#version 330 core\n\nlayout (std140) uniform Camera\n{\n    mat4 u_projView;\n    vec3 u_cameraPos;\n    float u_near;\n    float u_far;\n};\n///////////////////////////////////////////////////////////\nfloat rand(float c){\n	return fract(sin(c * 12.9898) * 43758.5453);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat rand(vec2 c){\n	return fract(sin(dot(c.xy, vec2(12.9898, 78.233))) * 43758.5453);\n}\n\n\n///////////////////////////////////////////////////////////\nvec2 rand2(vec2 st){\n    st = vec2(dot(st, vec2(127.1, 311.7)),\n              dot(st, vec2(269.5, 183.3)));\n    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);\n}\n\n\n///////////////////////////////////////////////////////////\nvec3 rand3(vec3 st){\n    st = vec3(dot(st, vec3(127.1, 311.7, 285.1)),\n              dot(st, vec3(269.5, 183.3, 161.3)),\n              dot(st, vec3(345.3, 102.9, 245.5)));\n    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat noise(vec2 st) {\n    vec2 i = floor(st);\n    vec2 f = fract(st);\n\n    vec2 u = f * f * (3.0 - 2.0 * f);\n\n    return mix( mix( dot( rand2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),\n                     dot( rand2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),\n                mix( dot( rand2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),\n                     dot( rand2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);\n}\n\n///////////////////////////////////////////////////////////////////////////////\n\nin vec2 v_texCoord;\n\nout vec4 f_color;\n\nuniform sampler2D u_color;\nuniform sampler2D u_normalShininess;\nuniform sampler2D u_specularReflectivity;\nuniform sampler2D u_depth;\n\nuniform mat4 u_invProjView;\n\nuniform float u_maxRaycastSteps = 30;\nuniform float u_stepToDepthRatio = 0.3f;\n\nconst float TRANSITION_RANGE = 0.05f;\n\n\n///////////////////////////////////////////////////////////////////////////////\nfloat getLinearDepth(float d)\n{\n    return 2.0 * u_near * u_far / (u_far + u_near - d * (u_far - u_near));\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nvec3 getFragPos(vec2 uv, out float depth)\n{\n    depth = 2.0f * texture(u_depth, uv).r - 1.0f;\n    vec4 clipPos = vec4(2.0f * uv - 1.0f, depth, 1.0f);\n    vec4 pos = u_invProjView * clipPos;\n\n    return pos.xyz / pos.w;\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nvec2 worldToUV(vec3 world)\n{\n    vec4 clipSpace = u_projView * vec4(world, 1.0f);\n    return clamp((clipSpace.xy / clipSpace.w) * 0.5f + 0.5f, 0.0f, 1.0f);\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nfloat getStepDistance(float depth)\n{\n    return depth * u_stepToDepthRatio / (u_maxRaycastSteps * u_near);\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nvoid main()\n{\n    float depth = 0.0f;\n    vec3 position = getFragPos(v_texCoord, depth);\n    vec3 color = texture(u_color, v_texCoord).rgb;\n    float reflectivity = texture(u_specularReflectivity, v_texCoord).a;\n    f_color = vec4(color, 1.0f);\n\n    // Check if the reflectivity is above 0 and not the skybox\n    if (depth >= 1.0f || reflectivity == 0.0f)\n        return;\n\n    // Get normal and reflected view vector\n    vec3 normal = texture(u_normalShininess, v_texCoord).rgb;\n    vec3 viewDir = normalize(position - u_cameraPos);\n    vec3 reflectedRay = normalize(reflect(viewDir, normal));\n    \n    // Raycast\n    vec2 rayTexCoord = v_texCoord;\n    float rayDepth = getLinearDepth(depth);\n    vec3 rayPoint = position;\n    \n    rayPoint += reflectedRay * 2.0f;\n\n\n    f_color = vec4(worldToUV(rayPoint), 0.0f, 1.0f);\n}"
#endif