#version 330 core

///////////////////////////////////////////////////////////////////////////////

in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D u_texture;
uniform float u_gamma;

///////////////////////////////////////////////////////////////////////////////

void main()
{
    vec4 result = texture(u_texture, v_texCoord);

    // Hdr and gamma correction
    f_color.rgb = pow(vec3(1.0f) - exp(-result.rgb), vec3(1.0f / u_gamma));
    f_color.a = result.a;
}