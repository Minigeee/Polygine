#version 330 core

in vec2 g_texCoord;
in vec4 g_color;

out vec4 f_color;

uniform sampler2D u_texture;
uniform bool u_hasTexture;


///////////////////////////////////////////////////////////
void main()
{
    if (u_hasTexture)
    {
        vec4 texColor = texture(u_texture, g_texCoord);

        float factor = (g_color.r * g_color.g * g_color.b) / 3.0f;
        factor = 0.08f * factor + 0.28f;
        factor = factor * texColor.a + (1.0f - factor);
        f_color.rgb = mix(texColor.rgb, texColor.rgb * g_color.rgb, factor);
        f_color.a = texColor.a * g_color.a;
    }
}