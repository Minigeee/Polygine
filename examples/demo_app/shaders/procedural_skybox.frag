#version 330 core

in vec3 v_fragPos;

out vec4 f_color;

uniform vec3 u_zenithColor;
uniform vec3 u_horizonColor;
uniform vec3 u_bloomColor;
uniform float u_bloomStrength;

uniform float u_topRadius;
uniform float u_botRadius;
uniform float u_radius;

uniform vec3 u_lightDir;

const float PI = 3.1415926535;


///////////////////////////////////////////////////////////
float phaseFunction_R(float nu)
{
  float k = 3.0f / (16.0f * PI);
  return k * (1.0f + nu * nu * 0.5f);
}


///////////////////////////////////////////////////////////
float phaseFunction_M(float g, float nu)
{
    float k = 3.0f / (8.0f * PI) * (1.0f - g * g) / (2.0f + g * g);
    return k * (1.0f + nu * nu) / pow(1.0f + g * g - 2.0f * g * nu, 1.5f);
}


///////////////////////////////////////////////////////////
void main()
{
    // Get the view direction
    vec3 viewDir = normalize(v_fragPos);

    // Get cosine of view direction
    float mu = dot(viewDir, vec3(0, 1, 0));
    float nu = dot(viewDir, -u_lightDir);

    // Get distance to top of atmosphere
    float r = u_radius;
    float d = -r * mu + sqrt(r * r * (mu * mu - 1.0f) + u_topRadius * u_topRadius);

    // Get min and max distances
    float d_min = u_topRadius - r;
    float distToHorizon = sqrt(r * r - u_botRadius * u_botRadius);
    float distBotToTop = sqrt(u_topRadius * u_topRadius - u_botRadius * u_botRadius);
    float d_max = distToHorizon + distBotToTop;

    // Calculate mix factor
    float factor = clamp((d - d_min) / (d_max - d_min), 0.0f, 1.0f);
    vec3 color = mix(u_zenithColor, u_horizonColor, factor);
    color *= phaseFunction_R(nu) * 12.0f;

    // Bloom (mie scattering) effect
    color += u_bloomColor * phaseFunction_M(u_bloomStrength, nu) * 0.5f;

    // Final color
    f_color.rgb = color;
}