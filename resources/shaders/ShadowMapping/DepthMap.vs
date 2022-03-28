
#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;


layout(std140) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    mat4 lightSpaceMatrice;
};

layout(std140) uniform GlobalPositions
{
    vec3 camPos;
    vec3 lightPos;
    vec2 lightNearFar;
};

uniform mat4 model;

out VS_OUT
{
    vec2 near_far;
    vec3 light_pos;
    vec4 frag_pos;
    vec2 lightDistance;
}vs_out;

void main()
{
    vs_out.near_far = lightNearFar;
    vs_out.light_pos = lightPos;
    vs_out.frag_pos = model * vec4(position, 1.0);

    //gl_FragDepth = gl_FragCoord.z;
    float lightDistance = length(vs_out.frag_pos.xyz - vs_out.light_pos);
    lightDistance /= vs_out.near_far.y;
    vs_out.lightDistance = vec2(lightDistance,lightDistance);

    gl_Position = lightSpaceMatrice * vs_out.frag_pos;
}