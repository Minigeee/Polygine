#ifndef SHADER_DEBUG_PHYSICS_FRAG
#define SHADER_DEBUG_PHYSICS_FRAG "#version 330 core\n\nin vec3 v_color;\n\nout vec4 f_color;\n\nvoid main()\n{\n    f_color = vec4(v_color, 1.0f);\n}"
#endif