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

uniform float u_maxDistance;
uniform float u_stepSize;
uniform float u_thickness;
uniform float u_maxDepthDiff;
uniform float u_noiseFactor;
uniform float u_fresnelFactor;


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
    return (clipSpace.xyz / clipSpace.w) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////////////////////////
vec3 clampUvRay(vec3 uv1, vec3 uv2)
{
    float xFactor = 1.0f;
    if (uv2.x > 1.0f)
        xFactor = (1.0f - uv1.x) / (uv2.x - uv1.x);
    else if (uv2.x < -1.0f)
        xFactor = (-1.0f - uv1.x) / (uv2.x - uv1.x);
        
    float yFactor = 1.0f;
    if (uv2.y > 1.0f)
        yFactor = (1.0f - uv1.y) / (uv2.y - uv1.y);
    else if (uv2.x < -1.0f)
        yFactor = (-1.0f - uv1.y) / (uv2.y - uv1.y);

    float factor = clamp(min(xFactor, yFactor), 0.0f, 1.0f);
    vec2 newUv = mix(uv1.xy, uv2.xy, factor);
    float newZ = (uv1.z * uv2.z) / mix(uv2.z, uv1.z, factor);

    return clamp(vec3(newUv, newZ), 0.0f, 1.0f);
}


///////////////////////////////////////////////////////////////////////////////
bool raycast(vec3 position, float depth, vec3 ray, out vec3 rayTexCoord)
{
    // Get endpoints in texture coords
    vec3 startTexCoords = vec3(v_texCoord, depth);
    vec3 endTexCoords = worldToUV(position + u_maxDistance * ray);
    endTexCoords = clampUvRay(startTexCoords, endTexCoords);

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
    rayTexCoord = startTexCoords;
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
        depth = texture(u_depth, rayTexCoord.xy).r;

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
        float depth = 0.0f;

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
            dot(ray, hitNormal) < 0.0f;
    }

    return hit;
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

    // Offset position by a random amount
    position += rand3(position) * u_noiseFactor - 0.5f * u_noiseFactor;

    // Get normal and reflected view vector
    vec3 normal = texture(u_normalShininess, v_texCoord).rgb;
    vec3 viewDir = normalize(position - u_cameraPos);
    vec3 reflectedRay = normalize(reflect(viewDir, normal));
    
    // Raycast
    vec3 rayTexCoord;
    bool hit = raycast(position, depth, reflectedRay, rayTexCoord);

    // Get color of reflection
    vec3 reflColor;
    if (hit)
        reflColor = texture(u_color, rayTexCoord.xy).rgb;

    else
    {
        // Take color from skybox
        if (u_usesProceduralSkybox)
        {
            // Make sure reflected ray doesn't go below horizon
            if (reflectedRay.y < 0.0f)
                reflectedRay = normalize(vec3(reflectedRay.x, 0.0f, reflectedRay.z));

            reflColor = getSkyColor(reflectedRay);
        }
    }

    // Apply the fresnel effect
    float reflFactor = (1.0f - pow(max(dot(-viewDir, normal), 0.0f), u_fresnelFactor)) * reflectivity;
    reflColor = mix(color, reflColor, reflFactor);

    f_color = vec4(reflColor, 1.0f);
}