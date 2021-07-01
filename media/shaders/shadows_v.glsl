
#include "common.glsl"
#include "shadows.glsl"

// Set up shadows in the vertex shader

///////////////////////////////////////////////////////////

#ifndef DEFERRED_SHADING
out vec4 v_clipSpacePos;
out vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];
#else
vec4 v_clipSpacePos;
vec4 v_lightClipSpacePos[MAX_NUM_SHADOW_MAPS];
#endif


///////////////////////////////////////////////////////////
void calcShadowClipSpace(vec4 worldPos)
{
    #ifndef DEFERRED_SHADING
    v_clipSpacePos = gl_Position;
    #else
    v_clipSpacePos = u_projView * worldPos;
    #endif

    // Calculate light space positions
    for (int i = 0; i < MAX_NUM_DIR_LIGHTS; ++i)
    {
        if (u_shadowsEnabled[i])
        {
            int start = i * MAX_NUM_SHADOW_CASCADES;
            int end = start + MAX_NUM_SHADOW_CASCADES;

            for (int j = start; j < end; ++j)
                v_lightClipSpacePos[j] = u_lightProjViews[j] * worldPos;
        }
    }
}