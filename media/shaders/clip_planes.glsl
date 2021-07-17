
///////////////////////////////////////////////////////////

uniform vec4 u_clipPlanes[8];
uniform int u_numClipPlanes;

float gl_ClipDistance[8];

///////////////////////////////////////////////////////////

void applyClipPlanes(vec3 pos)
{
    for (int i = 0; i < u_numClipPlanes; ++i)
        gl_ClipDistance[i] = dot(u_clipPlanes[i], vec4(pos, 1.0f));
}