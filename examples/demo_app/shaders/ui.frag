#version 330 core

in vec2 g_texCoord;
in vec4 g_color;

out vec4 f_color;

uniform sampler2D u_texture;
uniform bool u_hasTexture;
uniform bool u_flippedUv;


///////////////////////////////////////////////////////////
void main()
{
    f_color = g_color;
    if (u_hasTexture)
        f_color *= texture(u_texture, g_texCoord);
}