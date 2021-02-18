#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 v_position[];
in vec3 v_normal[];
in vec3 v_color[];
in float v_density[];
in float v_spacing[];

out vec3 g_position;
out vec3 g_normal;
out vec3 g_frontNormal;
out vec4 g_color;
out float g_dist;

uniform mat4 u_projView;
uniform vec3 u_cameraPos;

uniform float u_grassRadius;
uniform float u_grassSpacing;
uniform float u_grassWidth;
uniform float u_grassHeight;
const vec2 u_windDir = vec2(1, 0);

uniform float u_time;

///////////////////////////////////////////////////////////

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

///////////////////////////////////////////////////////////

void main()
{
    // Calculate a density value
    float density = rand(2.5f * v_position[0].xz + 0.1f);
    if (density > v_density[0])
    {
        EndPrimitive();
        return;
    }

    g_position = v_position[0];
    g_normal = v_normal[0];
    g_color.rgb = v_color[0];
    g_dist = distance(g_position.xz, u_cameraPos.xz);

    vec3 right = normalize(vec3(rand(g_position.xz) - 0.5f, 0.0f, rand(g_position.zx) - 0.5f));
    vec3 up = normalize(vec3(rand(2.0f * g_position.xz) * 1.4f - 0.7f, 1.0f, rand(2.0f * g_position.zx) * 1.4f - 0.7f));
    float grassWidth = 0.5f * u_grassWidth * (v_spacing[0] / u_grassSpacing);
    float grassHeight = (rand(2.0f * g_position.xz) * 0.8f + 0.6f) * u_grassHeight;

    float fadeFactor = clamp(1.0f - (g_dist - (u_grassRadius - 8.0f)) / 8.0f, 0.0f, 1.0f);
    grassWidth *= fadeFactor;
    grassHeight *= fadeFactor;
    g_color.a = fadeFactor;

    float windStrength = noise(v_position[0].xz + vec2(u_time, 0.0f) * 2.0f) * 0.6f;
    vec3 windOffset = vec3(u_windDir.x, 0, u_windDir.y) * windStrength;
    g_normal = normalize(g_normal + windOffset.xyz * 1.0f);
    g_frontNormal = normalize(cross(right * grassWidth, up * grassHeight + windOffset));

    vec3 worldPos = v_position[0] - right * grassWidth;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();
    
    worldPos = v_position[0] + right * grassWidth;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();
    
    worldPos = v_position[0] - right * grassWidth * 0.8f + 0.3f * up * grassHeight + 0.2f * windOffset;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();
    
    worldPos = v_position[0] + right * grassWidth * 0.8f + 0.3f * up * grassHeight + 0.2f * windOffset;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();
    
    worldPos = v_position[0] - right * grassWidth * 0.5f + 0.6f * up * grassHeight + 0.5f * windOffset;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();
    
    worldPos = v_position[0] + right * grassWidth * 0.5f + 0.6f * up * grassHeight + 0.5f * windOffset;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();
    
    worldPos = v_position[0] - right * grassWidth * 0.5f + up * grassHeight + 1.0f * windOffset;
    gl_Position = u_projView * vec4(worldPos, 1.0f);
    EmitVertex();

    EndPrimitive();
}