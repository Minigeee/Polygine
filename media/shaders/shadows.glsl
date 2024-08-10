
#include "common.glsl"

layout (std140) uniform Shadows
{
    uniform mat4 u_lightProjViews[MAX_NUM_SHADOW_MAPS];
    uniform float u_shadowDists[MAX_NUM_SHADOW_MAPS];
    uniform float u_shadowStrengths[MAX_NUM_DIR_LIGHTS];
    uniform int u_numShadowCascades[MAX_NUM_DIR_LIGHTS];
    uniform bool u_shadowsEnabled[MAX_NUM_DIR_LIGHTS];
};