#version 330 core

in vec2 g_texCoord;
in vec4 g_color;

out vec4 f_color;

uniform sampler2D u_texture;
uniform bool u_hasTexture;


///////////////////////////////////////////////////////////
void main()
{
    f_color = g_color;
    if (u_hasTexture)
    {
        vec4 texColor = texture(u_texture, g_texCoord);
        float factor = 0.35f * texColor.a + 0.65f;
        f_color.rgb = mix(texColor.rgb, texColor.rgb * g_color.rgb, factor);
        f_color.a = texColor.a * g_color.a;
    }
}