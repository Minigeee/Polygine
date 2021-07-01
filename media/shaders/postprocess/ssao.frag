#version 330 core

#include "../camera.glsl"
#include "../noise.glsl"

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_colorTexture;
uniform sampler2D u_depthTexture;

uniform mat4 u_invProjView;

uniform float u_radius;
uniform float u_bias;
uniform float u_range;
uniform float u_falloff;
uniform float u_intensity;
uniform float u_noiseFactor;

const float TRANSITION_RANGE = 0.05f;


///////////////////////////////////////////////////////////////////////////////
float getLinearDepth(float d)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * u_near * u_far / (u_far + u_near - z_n * (u_far - u_near));
}


///////////////////////////////////////////////////////////////////////////////
vec3 getFragPos(vec2 uv, out float depth)
{
    depth = 2.0f * texture(u_depthTexture, uv).r - 1.0f;
    vec4 clipPos = vec4(2.0f * uv - 1.0f, depth, 1.0f);
    vec4 pos = u_invProjView * clipPos;

    return pos.xyz / pos.w;
}


///////////////////////////////////////////////////////////////////////////////
vec3 getNormal(vec3 position)
{
    vec2 pixelSize = 1.0f / textureSize(u_depthTexture, 0);

    float depth;
    vec3 p10 = getFragPos(v_texCoord + vec2(-1, 0) * pixelSize, depth);
    vec3 p12 = getFragPos(v_texCoord + vec2(1, 0) * pixelSize, depth);
    vec3 p01 = getFragPos(v_texCoord + vec2(0, 1) * pixelSize, depth);
    vec3 p21 = getFragPos(v_texCoord + vec2(0, -1) * pixelSize, depth);

    // Change the normal sampling locations if the difference in distances is too large
    float d10 = distance(p10, position);
    float d12 = distance(p12, position);
    float d01 = distance(p01, position);
    float d21 = distance(p21, position);

    const float threshold = 4.0f;

    if (d10 / d12 > threshold)
        p10 = position;
    else if (d12 / d10 > threshold)
        p12 = position;
        
    if (d01 / d21 > threshold)
        p01 = position;
    else if (d21 / d01 > threshold)
        p21 = position;

    return normalize(cross(p12 - p10, p01 - p21));
}


///////////////////////////////////////////////////////////////////////////////
void main()
{
    float depth = 0.0f;
    vec3 position = getFragPos(v_texCoord, depth);
    vec3 normal = getNormal(position);
    depth = getLinearDepth(depth * 0.5f + 0.5f);

    f_color = texture(u_colorTexture, v_texCoord);

    if (depth > u_range)
        return;

    const int samples = 16;
    vec3 sampleSphere[samples] = vec3[](
        vec3( 0.5381, 0.1856,-0.4319), vec3( 0.1379, 0.2486, 0.4430),
        vec3( 0.3371, 0.5679,-0.0057), vec3(-0.6999,-0.0451,-0.0019),
        vec3( 0.0689,-0.1598,-0.8547), vec3( 0.0560, 0.0069,-0.1843),
        vec3(-0.0146, 0.1402, 0.0762), vec3( 0.0100,-0.1924,-0.0344),
        vec3(-0.3577,-0.5301,-0.4358), vec3(-0.3169, 0.1063, 0.0158),
        vec3( 0.0103,-0.5869, 0.0046), vec3(-0.0897,-0.4940, 0.3287),
        vec3( 0.7119,-0.0154,-0.0918), vec3(-0.0533, 0.0596,-0.5411),
        vec3( 0.0352,-0.0631, 0.5460), vec3(-0.4776, 0.2847,-0.0271)
    );

    float occlusion = 0.0f;
    for (int i = 0; i < samples; ++i)
    {
        vec3 samplePos = position + u_radius * sampleSphere[i];
        vec3 noiseOffset = (rand3(samplePos) - 0.5f) * u_radius * u_noiseFactor;
        vec3 sphereOffset = u_radius * sampleSphere[i] + noiseOffset;
        samplePos = position + sign(dot(sphereOffset, normal)) * sphereOffset;

        vec4 uv4 = u_projView * vec4(samplePos, 1.0f);
        vec3 samplePosClipSpace = (uv4.xyz / uv4.w) * 0.5f + 0.5f;

        float sampleDepth = getLinearDepth(texture(u_depthTexture, samplePosClipSpace.xy).r);

        float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(depth - sampleDepth));
        occlusion += smoothstep(0.0f, TRANSITION_RANGE, getLinearDepth(samplePosClipSpace.z) + u_bias - sampleDepth) * rangeCheck;
    }

    occlusion = 1.0f - clamp(occlusion / samples * u_intensity, 0.0f, 1.0f) / (1.0f + u_falloff * depth);
    f_color.rgb *= vec3(occlusion);
}