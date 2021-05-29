#version 330 core

#include "../noise.glsl"

// Uses code based off of:
// * https://www.shadertoy.com/view/4sX3Rs
// * https://github.com/Tomius/LoD

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_texture;
uniform vec2 u_screenSize;
uniform vec2 u_lightPos;

uniform float u_intensity;
uniform vec3 u_color;
uniform float u_luminosityFactor;


///////////////////////////////////////////////////////////////////////////////
vec3 lensFlare(vec2 uv, vec2 pos)
{
	vec2 uvd = uv * length(uv);

	float f21 = max(0.75f / (1.0f + 32.0f * pow(length(uvd + 0.8f * pos), 2.0f)), 0.0f) * 0.25f;
	float f22 = max(0.75f / (1.0f + 32.0f * pow(length(uvd + 0.85f * pos), 2.0f)), 0.0f) * 0.23f;
	float f23 = max(0.75f / (1.0f + 32.0f * pow(length(uvd + 0.9f * pos), 2.0f)), 0.0f) * 0.21f;

	vec2 uvx = mix(uv, uvd, -0.5f);
	float f41 = max(0.01f - pow(length(uvx + 0.4f * pos), 2.4f), 0.0) * 6.0f;
	float f42 = max(0.01f - pow(length(uvx + 0.45f * pos), 2.4f), 0.0) * 5.0f;
	float f43 = max(0.01f - pow(length(uvx + 0.5f * pos), 2.4 ), 0.0f) * 3.0f;

	uvx = mix(uv, uvd, -0.4f);
	float f51 = max(0.01f - pow(length(uvx + 0.2f * pos), 5.5f), 0.0f) * 2.0f;
	float f52 = max(0.01f - pow(length(uvx + 0.4f * pos), 5.5f), 0.0f) * 2.0f;
	float f53 = max(0.01f - pow(length(uvx + 0.6f * pos), 5.5f), 0.0) * 2.0f;

	uvx = mix(uv, uvd, -0.5f);
	float f61 = max(0.01f - pow(length(uvx - 0.3f * pos), 1.6f), 0.0f) * 6.0f;
	float f62 = max(0.01f - pow(length(uvx - 0.325f * pos), 1.6f), 0.0f) * 3.0f;
	float f63 = max(0.01f - pow(length(uvx - 0.35f * pos), 1.6f), 0.0f) * 5.0f;

	vec3 c = vec3(0.0f);
	c.r += f21 + f41 + f51 + f61;
	c.g += f22 + f42 + f52 + f62;
	c.b += f23 + f43 + f53 + f63;

	return 1.25f * c;
}


///////////////////////////////////////////////////////////////////////////////
vec3 cc(vec3 color, float factor, float factor2)
{
	float w = color.x+color.y+color.z;
	return mix(color,vec3(w)*factor,w*factor2);
}


///////////////////////////////////////////////////////////////////////////////
void main()
{
	vec2 uv = v_texCoord - 0.5f;
	uv.x *= u_screenSize.x / u_screenSize.y;
  	vec2 lightPos = u_lightPos.xy * 0.5f;
  	lightPos.x *= u_screenSize.x / u_screenSize.y;

	vec2 lightTexCoord = u_lightPos.xy * 0.5f + 0.5f;
	vec3 lightFragColor = texture(u_texture, lightTexCoord).rgb;
	float lightLuminosity = dot(vec3(0.299f, 0.587f, 0.114f), lightFragColor);
	float luminosityFactor = lightLuminosity * u_luminosityFactor;

    vec3 color = u_color * lensFlare(uv, lightPos) * u_intensity * luminosityFactor;
	color = cc(color, 0.5f, 0.1f);
	f_color = texture(u_texture, v_texCoord) + vec4(color, 1.0f);
}