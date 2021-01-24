#version 330 core

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_color;
uniform sampler2D u_depth;

uniform mat4 u_invProjView;
uniform vec3 u_cameraPos;

uniform vec3 u_fogColor;
uniform float u_fogDensity;
uniform float u_scatterStrength;
uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform bool u_applyToSkybox;


///////////////////////////////////////////////////////////////////////////////
vec3 getFragPos(vec2 uv, out float depth)
{
    depth = 2.0f * texture(u_depth, uv).r - 1.0f;
    vec4 clipPos = vec4(2.0f * uv - 1.0f, depth, 1.0f);
    vec4 pos = u_invProjView * clipPos;

    return pos.xyz / pos.w;
}


///////////////////////////////////////////////////////////
float phaseFunction_M(float g, float nu)
{
    float k = 3.0f / (8.0f * 3.1415926535f) * (1.0f - g * g) / (2.0f + g * g);
    return k * (1.0f + nu * nu) / pow(1.0f + g * g - 2.0f * g * nu, 1.5f);
}


///////////////////////////////////////////////////////////////////////////////
void main()
{
    vec3 color = texture(u_color, v_texCoord).rgb;
    
    float depth = 0.0f;
    vec3 fragPos = getFragPos(v_texCoord, depth);

    if (depth < 1.0f || u_applyToSkybox)
    {
        vec3 viewDir = fragPos - u_cameraPos;
        float dist = length(viewDir);
        viewDir = normalize(viewDir);
        
        float nu = dot(viewDir, -u_lightDir);

        float factor = pow(1.0f - u_fogDensity, dist);
        f_color.rgb = mix(u_fogColor, color, factor);
        
        // Check if the sun is intersecting
        f_color.rgb += u_lightColor * phaseFunction_M(0.8f, nu) * u_scatterStrength * (1.0f - factor);
    }
    else
        f_color.rgb = color;

    f_color.a = 1.0f;
}