#version 330 core

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

layout (location = 0) out vec4 f_color;
layout (location = 1) out float f_depth;

uniform sampler2D u_texture;
uniform sampler2D u_depth;
uniform bool u_hasDepth;

///////////////////////////////////////////////////////////////////////////////

void main()
{
    f_color = texture(u_texture, v_texCoord);
    if (u_hasDepth)
        f_depth = texture(u_depth, v_texCoord).r;
}