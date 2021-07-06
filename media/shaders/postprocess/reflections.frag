#version 330 core

#include "../camera.glsl"
#include "../noise.glsl"

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_color;
uniform sampler2D u_normalShininess;
uniform sampler2D u_specularReflectivity;
uniform sampler2D u_depth;

uniform mat4 u_invProjView;

uniform float u_maxDistance = 15.0f;
uniform float u_stepSize = 15.0f;
uniform float u_thickness = 0.5f;
uniform float u_maxDepthDiff = 0.02f;
uniform float u_noiseFactor = 0.004f;


///////////////////////////////////////////////////////////////////////////////
float getLinearDepth(float d)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * u_near * u_far / (u_far + u_near - z_n * (u_far - u_near));
}


///////////////////////////////////////////////////////////////////////////////
vec3 getFragPosFromDepth(vec2 uv, float depth)
{
    vec4 clipPos = vec4(2.0f * uv - 1.0f, 2.0f * depth - 1.0f, 1.0f);
    vec4 pos = u_invProjView * clipPos;

    return pos.xyz / pos.w;
}


///////////////////////////////////////////////////////////////////////////////
vec3 getFragPos(vec2 uv, out float depth)
{
    depth = texture(u_depth, uv).r;
    return getFragPosFromDepth(uv, depth);
}


///////////////////////////////////////////////////////////////////////////////
vec3 worldToUV(vec3 world)
{
    vec4 clipSpace = u_projView * vec4(world, 1.0f);
    return clamp((clipSpace.xyz / clipSpace.w) * 0.5f + 0.5f, 0.0f, 1.0f);
}


///////////////////////////////////////////////////////////////////////////////
void main()
{
    float depth = 0.0f;
    vec3 position = getFragPos(v_texCoord, depth);
    vec3 color = texture(u_color, v_texCoord).rgb;
    float reflectivity = texture(u_specularReflectivity, v_texCoord).a;
    f_color = vec4(color, 1.0f);

    // Check if the reflectivity is above 0 and not the skybox
    if (depth >= 1.0f || reflectivity == 0.0f)
        return;

    // Get normal and reflected view vector
    vec3 normal = texture(u_normalShininess, v_texCoord).rgb;
    vec3 viewDir = normalize(position - u_cameraPos);
    vec3 reflectedRay = normalize(reflect(viewDir, normal));

    // Get endpoints in texture coords
    vec3 startTexCoords = vec3(v_texCoord, depth);
    vec3 endTexCoords = worldToUV(position + u_maxDistance * reflectedRay);

    // Calculate increment size
    vec2 texSize = textureSize(u_depth, 0).xy;
    float segmentLength = distance(endTexCoords.xy, startTexCoords.xy);
    vec2 increment = u_stepSize / texSize * ((endTexCoords.xy - startTexCoords.xy) / segmentLength);
    float incrementLength = length(increment);

    if (incrementLength > segmentLength)
    {
        increment *= segmentLength / incrementLength;
        incrementLength = segmentLength;
    }
    
    // Raycast
    vec3 rayTexCoord = startTexCoords;
    bool hit = false;

    // Keep track of increment endpoints for binary search later
    vec3 endpoint1, endpoint2;

    for (float rayLength = 0.0f; rayLength < segmentLength; rayLength += incrementLength)
    {
        rayTexCoord.xy += increment;

        // Get percentage of the ray that has been travelled
        float percent = (rayTexCoord.x - startTexCoords.x) / (endTexCoords.x - startTexCoords.x);

        // Get ray depth by interpolating depth
        rayTexCoord.z = (startTexCoords.z * endTexCoords.z) / mix(endTexCoords.z, startTexCoords.z, percent);

        // Get depth of the pixel
        vec2 offset = rand2(rayTexCoord.xy) * u_noiseFactor - 0.5f * u_noiseFactor;
        float depth = texture(u_depth, rayTexCoord.xy + offset).r;

        // Get linear depths
        float rayDepthLinear = getLinearDepth(rayTexCoord.z);
        float depthLinear = getLinearDepth(depth);
        float depthDiff = rayDepthLinear - depthLinear;

        // Check for intersection
        if (depthDiff > 0.0f && depthDiff < u_thickness)
        {
            endpoint2 = rayTexCoord;
            hit = true;
            break;
        }

        endpoint1 = rayTexCoord;
    }

    if (hit)
    {
        // Binary search to find accurate position
        for (int i = 0; i < 5; ++i)
        {
            // Get midpoint
            vec3 midpoint = mix(endpoint1, endpoint2, 0.5f);
            midpoint.z = (endpoint1.z * endpoint2.z) / mix(endpoint2.z, endpoint1.z, 0.5f);
            rayTexCoord = midpoint;

            // Get depth of the pixel
            depth = texture(u_depth, midpoint.xy).r;

            if (depth < rayTexCoord.z)
                // The fragment is in front of the ray
                endpoint2 = midpoint;
            else
                // The ray is in front of the fragment
                endpoint1 = midpoint;
        }

        // Make sure the hit is valid
        vec3 hitNormal = texture(u_normalShininess, rayTexCoord.xy).xyz;

        hit =
            abs(getLinearDepth(depth) - getLinearDepth(rayTexCoord.z)) < u_maxDepthDiff &&
            dot(reflectedRay, hitNormal) < 0.0f;
    }

    f_color = vec4(hit ? texture(u_color, rayTexCoord.xy).rgb : vec3(0.0f), 1.0f);
}