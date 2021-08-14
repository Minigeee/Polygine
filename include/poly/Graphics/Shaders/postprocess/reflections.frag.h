#ifndef SHADER_POSTPROCESS_REFLECTIONS_FRAG
#define SHADER_POSTPROCESS_REFLECTIONS_FRAG "#version 330 core\n\nlayout (std140) uniform Camera\n{\n    mat4 u_projView;\n    vec3 u_cameraPos;\n    float u_near;\n    float u_far;\n};\n///////////////////////////////////////////////////////////\nfloat rand(float c){\n	return fract(sin(c * 12.9898) * 43758.5453);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat rand(vec2 c){\n	return fract(sin(dot(c.xy, vec2(12.9898, 78.233))) * 43758.5453);\n}\n\n\n///////////////////////////////////////////////////////////\nvec2 rand2(vec2 st){\n    st = vec2(dot(st, vec2(127.1, 311.7)),\n              dot(st, vec2(269.5, 183.3)));\n    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);\n}\n\n\n///////////////////////////////////////////////////////////\nvec3 rand3(vec3 st){\n    st = vec3(dot(st, vec3(127.1, 311.7, 285.1)),\n              dot(st, vec3(269.5, 183.3, 161.3)),\n              dot(st, vec3(345.3, 102.9, 245.5)));\n    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat noise(vec2 st) {\n    vec2 i = floor(st);\n    vec2 f = fract(st);\n\n    vec2 u = f * f * (3.0 - 2.0 * f);\n\n    return mix( mix( dot( rand2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),\n                     dot( rand2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),\n                mix( dot( rand2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),\n                     dot( rand2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);\n}\nuniform vec3 u_zenithColor;\nuniform vec3 u_horizonColor;\nuniform vec3 u_groundColor;\nuniform vec3 u_scatterColor;\nuniform float u_scatterFactor;\nuniform float u_lightStrength;\n\nuniform float u_topRadius;\nuniform float u_botRadius;\nuniform float u_radius;\n\nuniform vec3 u_lightDir;\n\nconst float PI = 3.1415926535;\n\n\n///////////////////////////////////////////////////////////\nfloat phaseFunction_R(float nu)\n{\n  float k = 3.0f / (16.0f * PI);\n  return k * (1.0f + nu * nu * 0.5f);\n}\n\n\n///////////////////////////////////////////////////////////\nfloat phaseFunction_M(float g, float nu)\n{\n    float k = 3.0f / (8.0f * PI) * (1.0f - g * g) / (2.0f + g * g);\n    return k * (1.0f + nu * nu) / pow(1.0f + g * g - 2.0f * g * nu, 1.5f);\n}\n\n\n///////////////////////////////////////////////////////////\nvec3 getSkyColor(vec3 viewDir)\n{\n    // Get cosine of view direction\n    float mu = dot(viewDir, vec3(0, 1, 0));\n    float nu = dot(viewDir, -u_lightDir);\n\n    // Get distance to top of atmosphere\n    float r = u_radius;\n    float d = -r * mu + sqrt(r * r * (mu * mu - 1.0f) + u_topRadius * u_topRadius);\n    float d_0 = -u_botRadius * mu + sqrt(\n        u_botRadius * u_botRadius * (mu * mu - 1.0f) + u_topRadius * u_topRadius);\n\n    // Get min and max distances\n    float d_min = u_topRadius - r;\n    float distToHorizon = sqrt(r * r - u_botRadius * u_botRadius);\n    float distBotToTop = sqrt(u_topRadius * u_topRadius - u_botRadius * u_botRadius);\n    float d_max = distToHorizon + distBotToTop;\n\n    // Fix ground intersections\n    bool intersectsGround = d > d_max;\n    if (intersectsGround)\n    {\n        d = -r * mu - sqrt(r * r * (mu * mu - 1.0f) + u_botRadius * u_botRadius);\n        d_0 = d;\n    }\n\n    // Calculate mix factor\n    float factor = clamp((d - d_min) / (d_max - d_min), 0.0f, 1.0f);\n    float heightFactor = 1.0f - clamp((r - u_botRadius) / (u_topRadius - u_botRadius), 0.0f, 1.0f);\n\n    vec3 horizonColor = mix(u_zenithColor, u_horizonColor, heightFactor);\n    vec3 color = mix(u_zenithColor, horizonColor, factor) * (d / d_0);\n    if (intersectsGround)\n        color = color * factor * 0.5f + u_groundColor;\n\n    color *= phaseFunction_R(nu) * u_lightStrength;\n\n    // scatter (mie scattering) effect\n    color += u_scatterColor * phaseFunction_M(u_scatterFactor, nu) * pow(d / d_max, 0.8f) * heightFactor;\n\n    return color;\n}\n\n///////////////////////////////////////////////////////////////////////////////\n\nin vec2 v_texCoord;\n\nout vec4 f_color;\n\nuniform sampler2D u_color;\nuniform sampler2D u_normalShininess;\nuniform sampler2D u_specularReflectivity;\nuniform sampler2D u_depth;\n\nuniform mat4 u_invProjView;\nuniform bool u_usesProceduralSkybox;\n\nuniform int u_maxSteps;\nuniform float u_stepSize;\nuniform float u_maxDepthDiff;\nuniform float u_fresnelFactor;\nuniform float u_fresnelFactorMin;\n\n\n///////////////////////////////////////////////////////////////////////////////\nvoid main()\n{\n    float depth = texture(u_depth, v_texCoord).r;\n    vec3 color = texture(u_color, v_texCoord).rgb;\n    float reflectivity = texture(u_specularReflectivity, v_texCoord).a;\n    f_color = vec4(color, 1.0f);\n\n    // Check if the reflectivity is above 0 and not the skybox\n    if (depth >= 1.0f || reflectivity == 0.0f)\n        return;\n\n    vec4 posNDC = vec4(2.0f * v_texCoord - 1.0f, 2.0f * depth - 1.0f, 1.0f);\n    vec4 posWorld = u_invProjView * posNDC;\n    posWorld /= posWorld.w;\n\n    // Get normal and reflected view vector\n    vec3 normal = texture(u_normalShininess, v_texCoord).rgb;\n    vec3 viewDir = normalize(posWorld.xyz - u_cameraPos);\n    vec3 rayWorld = normalize(reflect(viewDir, normal));\n    \n    // Calculate second ray point\n    vec4 farWorld = vec4(posWorld.xyz + rayWorld, 1.0f);\n    vec4 farNDC = u_projView * farWorld;\n    farNDC /= farNDC.w;\n    \n    // Calculate step vector\n    vec3 stepNDC = farNDC.xyz - posNDC.xyz;\n\n    // Only raycast for rays that point away from camera\n    vec3 reflColor;\n    bool hit = false;\n\n    if (true)\n    {\n        // Make step go one pixel in the direction the change is largest\n        vec2 dv = abs(stepNDC.xy * 0.5f * textureSize(u_depth, 0));\n        stepNDC = stepNDC / (dv.x > dv.y ? dv.x : dv.y);\n        \n        vec3 rayNDC = posNDC.xyz + stepNDC.xyz;\n        vec3 rayTex = rayNDC.xyz * 0.5f + 0.5f;\n        vec3 dRayTex = stepNDC * 0.5f * u_stepSize;\n\n        int nStep = 0;\n        while (nStep < u_maxSteps)\n        {\n            // Get depth at ray texture coord\n            float zDepth = texture(u_depth, rayTex.xy).r;\n\n            if (rayTex.z > zDepth && abs(rayTex.z - zDepth) < u_maxDepthDiff)\n            {\n                hit = true;\n                reflColor = texture(u_color, rayTex.xy).rgb;\n                break;\n            }\n\n            nStep++;\n            rayTex += dRayTex;\n\n            // Stop raycast\n            if (!(rayTex.x > 0.001 && rayTex.x < 0.999 && rayTex.y > 0.01 && rayTex.y < 0.999 && rayTex.z < 1.0f)) break;\n        }\n    }\n\n    // Use skybox if no hit was detected\n    if (!hit)\n    {\n        // Take color from skybox\n        if (u_usesProceduralSkybox)\n        {\n            // Make sure reflected ray doesn't go below horizon\n            if (rayWorld.y < 0.0f)\n                rayWorld = normalize(vec3(rayWorld.x, 0.0f, rayWorld.z));\n\n            reflColor = getSkyColor(rayWorld);\n        }\n    }\n\n    // Apply the fresnel effect\n    float reflFactor = (1.0f - pow(max(dot(-viewDir, normal), 0.0f), u_fresnelFactor)) * reflectivity;\n    reflFactor = max(reflFactor, u_fresnelFactorMin);\n    reflColor = mix(color, reflColor, reflFactor);\n\n    f_color = vec4(reflColor, 1.0f);\n}"
#endif