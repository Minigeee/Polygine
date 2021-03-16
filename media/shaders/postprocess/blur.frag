#version 330 core

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_texture;
uniform bool u_verticalBlur;
uniform int u_kernelSize;
uniform float u_weights[50];

///////////////////////////////////////////////////////////////////////////////

void main()
{             
    vec2 pixelSize = 1.0f / textureSize(u_texture, 0);
    vec3 result = texture(u_texture, v_texCoord).rgb * u_weights[0];
    
    if (u_verticalBlur)
    {
        for (int i = 1; i < u_kernelSize / 2; ++i)
        {
            result += texture(u_texture, v_texCoord + vec2(0.0, pixelSize.y * i)).rgb * u_weights[i];
            result += texture(u_texture, v_texCoord - vec2(0.0, pixelSize.y * i)).rgb * u_weights[i];
        }
    }
    else
    {
        for (int i = 1; i < u_kernelSize / 2; ++i)
        {
            result += texture(u_texture, v_texCoord + vec2(pixelSize.x * i, 0.0)).rgb * u_weights[i];
            result += texture(u_texture, v_texCoord - vec2(pixelSize.x * i, 0.0)).rgb * u_weights[i];
        }
    }

    f_color = vec4(result, 1.0);
}