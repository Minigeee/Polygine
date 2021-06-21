#ifndef SHADER_CUBEMAP_VERT
#define SHADER_CUBEMAP_VERT "#version 330 core\n\nlayout (location = 0) in vec3 a_position;\n\nout vec3 v_fragPos;\n\nuniform mat4 u_projView;\n\nvoid main()\n{\n    gl_Position =  u_projView * vec4(a_position, 1.0);\n    gl_Position = gl_Position.xyww;\n\n    v_fragPos = a_position;\n}"
#endif