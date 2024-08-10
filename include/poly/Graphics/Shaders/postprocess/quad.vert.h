#ifndef SHADER_POSTPROCESS_QUAD_VERT
#define SHADER_POSTPROCESS_QUAD_VERT "#version 330 core\n\nlayout (location = 0) in vec2 a_pos;\n\nout vec2 v_texCoord;\n\nvoid main()\n{\n    gl_Position = vec4(a_pos, 0.0f, 1.0f);\n    v_texCoord = a_pos * 0.5f + 0.5f;\n}"
#endif