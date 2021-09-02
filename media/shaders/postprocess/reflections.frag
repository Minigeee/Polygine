#version 330 core

#include "../camera.glsl"
#include "../noise.glsl"
#include "../procedural_skybox.glsl"

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_color;
uniform sampler2D u_normalShininess;
uniform sampler2D u_specularReflectivity;
uniform sampler2D u_depth;

uniform mat4 u_invProjView;
uniform bool u_usesProceduralSkybox;

uniform int u_maxSteps;
uniform float u_stepSize;
uniform float u_maxDepthDiff;
uniform float u_fresnelFactor;
uniform float u_fresnelFactorMin;


///////////////////////////////////////////////////////////////////////////////
void main()
{
    float depth = texture(u_depth, v_texCoord).r;
    vec3 color = texture(u_color, v_texCoord).rgb;
    float reflectivity = texture(u_specularReflectivity, v_texCoord).a;
    f_color = vec4(color, 1.0f);

    // Check if the reflectivity is above 0 and not the skybox
    if (depth >= 1.0f || reflectivity == 0.0f)
        return;

    vec4 posNDC = vec4(2.0f * v_texCoord - 1.0f, 2.0f * depth - 1.0f, 1.0f);
    vec4 posWorld = u_invProjView * posNDC;
    posWorld /= posWorld.w;

    // Get normal and reflected view vector
    vec3 normal = texture(u_normalShininess, v_texCoord).rgb;
    vec3 viewDir = normalize(posWorld.xyz - u_cameraPos);
    vec3 rayWorld = normalize(reflect(viewDir, normal));
    
    // Calculate second ray point
    vec4 farWorld = vec4(posWorld.xyz + rayWorld, 1.0f);
    vec4 farNDC = u_projView * farWorld;
    farNDC /= farNDC.w;
    
    // Calculate step vector
    vec3 stepNDC = farNDC.xyz - posNDC.xyz;

    // Only raycast for rays that point away from camera
    vec3 reflColor;
    bool hit = false;
    vec3 endpoint1, endpoint2;

    if (true)
    {
        float depthDiffFactor = 20.0f * pow(abs(normalize(stepNDC).z), 0.6f);

        // Make step go one pixel in the direction the change is largest
        vec2 dv = abs(stepNDC.xy * 0.5f * textureSize(u_depth, 0));
        stepNDC = stepNDC / (dv.x > dv.y ? dv.x : dv.y);
        
        vec3 rayNDC = posNDC.xyz + stepNDC.xyz;
        vec3 rayTex = rayNDC.xyz * 0.5f + 0.5f;
        vec3 dRayTex = stepNDC * 0.5f * u_stepSize;

        int nStep = 0;
        while (nStep < u_maxSteps)
        {
            // Get depth at ray texture coord
            float zDepth = texture(u_depth, rayTex.xy).r;

            if (rayTex.z > zDepth && abs(rayTex.z - zDepth) < u_maxDepthDiff * depthDiffFactor)
            {
                hit = true;
                endpoint2 = rayTex;
                break;
            }
            
            if (rayTex.z > 1.0f) break;
            
            endpoint1 = rayTex;

            nStep++;
            rayTex += dRayTex;

            // Stop raycast
            if (!(rayTex.x > 0.001 && rayTex.x < 0.999 && rayTex.y > 0.01 && rayTex.y < 0.999)) break;
        }
    }

    // Get precies intersect location
    if (hit)
    {
        vec3 midpoint;
        float zDepth;

        for (int i = 0; i < 3; ++i)
        {
            midpoint = mix(endpoint1, endpoint2, 0.5f);
            zDepth = texture(u_depth, midpoint.xy).r;

            if (midpoint.z > zDepth)
                endpoint2 = midpoint;
            else
                endpoint1 = midpoint;
        }

        midpoint = mix(endpoint1, endpoint2, 0.5f);
        zDepth = texture(u_depth, midpoint.xy).r;
        if (zDepth < 1.0f)
            reflColor = texture(u_color, midpoint.xy).rgb;
        else
            hit = false;
    }
    
    // Use skybox if no hit was detected
    if (!hit)
    {
        // Take color from skybox
        if (u_usesProceduralSkybox)
        {
            // Make sure reflected ray doesn't go below horizon
            if (rayWorld.y < 0.0f)
                rayWorld = normalize(vec3(rayWorld.x, 0.0f, rayWorld.z));

            reflColor = getSkyColor(rayWorld);
        }
    }

    // Apply the fresnel effect
    float reflFactor = (1.0f - pow(max(dot(-viewDir, normal), 0.0f), u_fresnelFactor)) * reflectivity;
    reflFactor = max(reflFactor, u_fresnelFactorMin);
    reflColor = mix(color, reflColor, reflFactor);

    f_color = vec4(reflColor, 1.0f);
}