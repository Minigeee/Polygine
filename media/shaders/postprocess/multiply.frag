#version 330 core

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_factor;

///////////////////////////////////////////////////////////////////////////////

void main()
{
    vec4 result1 = texture(u_texture1, v_texCoord);
    vec4 result2 = texture(u_texture2, v_texCoord);

    f_color.rgb = result1.rgb * result2.rgb * u_factor;
    f_color.a = max(result1.a, result2.a);
}