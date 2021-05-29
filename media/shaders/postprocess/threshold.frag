#version 330 core

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_texture;
uniform float u_threshold;
uniform float u_interval;

///////////////////////////////////////////////////////////////////////////////

void main()
{
    vec4 result = texture(u_texture, v_texCoord);
    
	const vec3 toLuma = vec3(0.299, 0.587, 0.114);
    float brightness = dot(result.rgb, toLuma);

    // If interval is zero or negative, use a hard threshold
    if (u_interval <= 0.0f && brightness < u_threshold)
        result.rgb = vec3(0.0f);

    else if (u_interval > 0.0f)
    {
        // Otherwise use a smoothstep
        float factor = smoothstep(u_threshold - u_interval, u_threshold, brightness);
        result.rgb *= factor;
    }

    f_color = result;
}