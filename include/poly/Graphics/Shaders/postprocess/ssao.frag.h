#ifndef SHADER_POSTPROCESS_SSAO_FRAG
#define SHADER_POSTPROCESS_SSAO_FRAG "#version 330 core\n\nlayout (std140) uniform Camera\n{\n    mat4 u_projView;\n    vec3 u_cameraPos;\n    float u_near;\n    float u_far;\n};\n///////////////////////////////////////////////////////////\nfloat rand(float c){\n	return fract(sin(c * 12.9898) * 43758.5453);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat rand(vec2 c){\n	return fract(sin(dot(c.xy, vec2(12.9898, 78.233))) * 43758.5453);\n}\n\n\n///////////////////////////////////////////////////////////\nvec2 rand2(vec2 st){\n    st = vec2(dot(st, vec2(127.1, 311.7)),\n              dot(st, vec2(269.5, 183.3)));\n    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);\n}\n\n\n///////////////////////////////////////////////////////////\nvec3 rand3(vec3 st){\n    st = vec3(dot(st, vec3(127.1, 311.7, 285.1)),\n              dot(st, vec3(269.5, 183.3, 161.3)),\n              dot(st, vec3(345.3, 102.9, 245.5)));\n    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat noise(vec2 st) {\n    vec2 i = floor(st);\n    vec2 f = fract(st);\n\n    vec2 u = f * f * (3.0 - 2.0 * f);\n\n    return mix( mix( dot( rand2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),\n                     dot( rand2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),\n                mix( dot( rand2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),\n                     dot( rand2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);\n}\n\n///////////////////////////////////////////////////////////////////////////////\n\nin vec2 v_texCoord;\n\nout vec4 f_color;\n\nuniform sampler2D u_colorTexture;\nuniform sampler2D u_depthTexture;\n\nuniform mat4 u_invProjView;\n\nuniform float u_radius;\nuniform float u_bias;\nuniform float u_range;\nuniform float u_falloff;\nuniform float u_intensity;\nuniform float u_noiseFactor;\n\nconst float TRANSITION_RANGE = 0.05f;\n\n\n///////////////////////////////////////////////////////////////////////////////\nfloat getLinearDepth(float d)\n{\n    float z_n = 2.0 * d - 1.0;\n    return 2.0 * u_near * u_far / (u_far + u_near - z_n * (u_far - u_near));\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nvec3 getFragPos(vec2 uv, out float depth)\n{\n    depth = 2.0f * texture(u_depthTexture, uv).r - 1.0f;\n    vec4 clipPos = vec4(2.0f * uv - 1.0f, depth, 1.0f);\n    vec4 pos = u_invProjView * clipPos;\n\n    return pos.xyz / pos.w;\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nvec3 getNormal(vec3 position)\n{\n    vec2 pixelSize = 1.0f / textureSize(u_depthTexture, 0);\n\n    float depth;\n    vec3 p10 = getFragPos(v_texCoord + vec2(-1, 0) * pixelSize, depth);\n    vec3 p12 = getFragPos(v_texCoord + vec2(1, 0) * pixelSize, depth);\n    vec3 p01 = getFragPos(v_texCoord + vec2(0, 1) * pixelSize, depth);\n    vec3 p21 = getFragPos(v_texCoord + vec2(0, -1) * pixelSize, depth);\n\n    // Change the normal sampling locations if the difference in distances is too large\n    float d10 = distance(p10, position);\n    float d12 = distance(p12, position);\n    float d01 = distance(p01, position);\n    float d21 = distance(p21, position);\n\n    const float threshold = 4.0f;\n\n    if (d10 / d12 > threshold)\n        p10 = position;\n    else if (d12 / d10 > threshold)\n        p12 = position;\n        \n    if (d01 / d21 > threshold)\n        p01 = position;\n    else if (d21 / d01 > threshold)\n        p21 = position;\n\n    return normalize(cross(p12 - p10, p01 - p21));\n}\n\n\n///////////////////////////////////////////////////////////////////////////////\nvoid main()\n{\n    float depth = 0.0f;\n    vec3 position = getFragPos(v_texCoord, depth);\n    vec3 normal = getNormal(position);\n    depth = getLinearDepth(depth * 0.5f + 0.5f);\n\n    f_color = texture(u_colorTexture, v_texCoord);\n\n    if (depth > u_range)\n        return;\n\n    const int samples = 16;\n    vec3 sampleSphere[samples] = vec3[](\n        vec3( 0.5381, 0.1856,-0.4319), vec3( 0.1379, 0.2486, 0.4430),\n        vec3( 0.3371, 0.5679,-0.0057), vec3(-0.6999,-0.0451,-0.0019),\n        vec3( 0.0689,-0.1598,-0.8547), vec3( 0.0560, 0.0069,-0.1843),\n        vec3(-0.0146, 0.1402, 0.0762), vec3( 0.0100,-0.1924,-0.0344),\n        vec3(-0.3577,-0.5301,-0.4358), vec3(-0.3169, 0.1063, 0.0158),\n        vec3( 0.0103,-0.5869, 0.0046), vec3(-0.0897,-0.4940, 0.3287),\n        vec3( 0.7119,-0.0154,-0.0918), vec3(-0.0533, 0.0596,-0.5411),\n        vec3( 0.0352,-0.0631, 0.5460), vec3(-0.4776, 0.2847,-0.0271)\n    );\n\n    float occlusion = 0.0f;\n    for (int i = 0; i < samples; ++i)\n    {\n        vec3 samplePos = position + u_radius * sampleSphere[i];\n        vec3 noiseOffset = (rand3(samplePos) - 0.5f) * u_radius * u_noiseFactor;\n        vec3 sphereOffset = u_radius * sampleSphere[i] + noiseOffset;\n        samplePos = position + sign(dot(sphereOffset, normal)) * sphereOffset;\n\n        vec4 uv4 = u_projView * vec4(samplePos, 1.0f);\n        vec3 samplePosClipSpace = (uv4.xyz / uv4.w) * 0.5f + 0.5f;\n\n        float sampleDepth = getLinearDepth(texture(u_depthTexture, samplePosClipSpace.xy).r);\n\n        float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(depth - sampleDepth));\n        occlusion += smoothstep(0.0f, TRANSITION_RANGE, getLinearDepth(samplePosClipSpace.z) + u_bias - sampleDepth) * rangeCheck * u_intensity;\n    }\n\n    occlusion = 1.0f - (occlusion / samples) / (1.0f + u_falloff * depth);\n    f_color.rgb *= vec3(occlusion);\n}"
#endif