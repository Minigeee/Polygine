#version 330 core

#include "../noise.glsl"

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_texture;
uniform bool u_verticalBlur;
uniform int u_kernelSize;
uniform float u_kernelSpacing;
uniform float u_noiseFactor;
uniform float u_weights[50];

///////////////////////////////////////////////////////////////////////////////

void main()
{             
    vec2 pixelSize = u_kernelSpacing / textureSize(u_texture, 0);
    vec3 result = texture(u_texture, v_texCoord).rgb * u_weights[0];
    
    if (u_verticalBlur)
    {
        for (int i = 1; i < u_kernelSize / 2; ++i)
        {
            vec2 tc1 = v_texCoord + vec2(0.0, pixelSize.y * i);
            vec2 tc2 = v_texCoord - vec2(0.0, pixelSize.y * i);
            tc1.y += (rand(tc1) - 0.5f) * pixelSize.y * u_noiseFactor;
            tc2.y += (rand(tc2) - 0.5f) * pixelSize.y * u_noiseFactor;

            result += texture(u_texture, tc1).rgb * u_weights[i];
            result += texture(u_texture, tc2).rgb * u_weights[i];
        }
    }
    else
    {
        for (int i = 1; i < u_kernelSize / 2; ++i)
        {
            vec2 tc1 = v_texCoord + vec2(pixelSize.x * i, 0.0);
            vec2 tc2 = v_texCoord - vec2(pixelSize.x * i, 0.0);
            tc1.x += (rand(tc1) - 0.5f) * pixelSize.x * u_noiseFactor;
            tc2.x += (rand(tc2) - 0.5f) * pixelSize.x * u_noiseFactor;

            result += texture(u_texture, tc1).rgb * u_weights[i];
            result += texture(u_texture, tc2).rgb * u_weights[i];
        }
    }

    f_color = vec4(result, 1.0);
}