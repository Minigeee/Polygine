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
        f_color = texColor;
    }
}