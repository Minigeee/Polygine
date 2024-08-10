#ifndef SHADER_UI_FRAG
#define SHADER_UI_FRAG "#version 330 core\n\nin vec2 g_texCoord;\nin vec4 g_color;\n\nout vec4 f_color;\n\nuniform sampler2D u_texture;\nuniform bool u_hasTexture;\n\n\n///////////////////////////////////////////////////////////\nvoid main()\n{\n    f_color = g_color;\n    if (u_hasTexture)\n        f_color *= texture(u_texture, g_texCoord);\n}"
#endif