
#include "common.glsl"

// Set up shadows in the vertex shader

///////////////////////////////////////////////////////////

out vec4 v_clipSpacePos;
out vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];

uniform mat4 u_lightProjViews[MAX_NUM_SHADOW_MAPS];
uniform int u_numShadows;


///////////////////////////////////////////////////////////
void calcShadowClipSpace(vec4 worldPos)
{
    v_clipSpacePos = gl_Position;

    // Calculate light space positions
    for (int i = 0; i < u_numShadows * MAX_NUM_SHADOW_CASCADES; ++i)
        v_lightClipSpacePos[i] = u_lightProjViews[i] * worldPos;
}