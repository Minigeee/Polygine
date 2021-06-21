#ifndef SHADER_SKYBOX_FRAG
#define SHADER_SKYBOX_FRAG "#version 330 core\n\nin vec3 v_fragPos;\n\nout vec4 f_color;\n\nuniform samplerCube u_cubemap;\n\n\n///////////////////////////////////////////////////////////\nvoid main()\n{\n    // Final color\n    f_color.rgb = texture(u_cubemap, v_fragPos).rgb;\n}"
#endif