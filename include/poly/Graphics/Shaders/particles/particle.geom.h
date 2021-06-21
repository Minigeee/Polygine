#ifndef SHADER_PARTICLES_PARTICLE_GEOM
#define SHADER_PARTICLES_PARTICLE_GEOM "#version 330 core\n\nlayout (std140) uniform Camera\n{\n    mat4 u_projView;\n    vec3 u_cameraPos;\n    float u_near;\n    float u_far;\n};\n\nlayout (points) in;\nlayout (triangle_strip, max_vertices = 4) out;\n\nin vec3 v_position[];\nin vec2 v_size[];\nin float v_rotation[];\nin vec2 v_texCoord[];\nin vec2 v_texSize[];\nin vec4 v_color[];\n\nout vec2 g_texCoord;\nout vec4 g_color;\n\nconst float PI = 3.1415926535;\n\n\n///////////////////////////////////////////////////////////\nvoid main()\n{\n    // Calculate rotation stuff\n    float c = cos(v_rotation[0] * PI / 180.0f);\n    float s = sin(v_rotation[0] * PI / 180.0f);\n\n    // Calculate the three local cardinal vectors of the particle\n    vec3 front = normalize(u_cameraPos - v_position[0]);\n    vec3 right = normalize(cross(vec3(0, 1, 0), front));\n    right = normalize(right * c + cross(front, right) * s + front * dot(front, right) * (1.0f - c));\n    vec3 up = normalize(cross(front, right));\n\n    // Get corner vertices\n    vec4 v1 = vec4(v_position[0] - right * v_size[0].x + up * v_size[0].y, 1.0f);\n    vec4 v2 = vec4(v_position[0] - right * v_size[0].x - up * v_size[0].y, 1.0f);\n    vec4 v3 = vec4(v_position[0] + right * v_size[0].x + up * v_size[0].y, 1.0f);\n    vec4 v4 = vec4(v_position[0] + right * v_size[0].x - up * v_size[0].y, 1.0f);\n    v1 = u_projView * v1;\n    v2 = u_projView * v2;\n    v3 = u_projView * v3;\n    v4 = u_projView * v4;\n\n    // Texture coords\n    vec2 t1 = v_texCoord[0];\n    vec2 t2 = v_texCoord[0] + vec2(v_texSize[0].x, 0.0f);\n    vec2 t3 = v_texCoord[0] + vec2(0.0f, v_texSize[0].y);\n    vec2 t4 = v_texCoord[0] + vec2(v_texSize[0].x, v_texSize[0].y);\n\n    // Emit vertices\n    gl_Position = v1;\n    g_texCoord = t3;\n    g_color = v_color[0];\n    EmitVertex();\n    \n    gl_Position = v2;\n    g_texCoord = t1;\n    EmitVertex();\n    \n    gl_Position = v3;\n    g_texCoord = t4;\n    EmitVertex();\n    \n    gl_Position = v4;\n    g_texCoord = t2;\n    EmitVertex();\n\n    EndPrimitive();\n}"
#endif