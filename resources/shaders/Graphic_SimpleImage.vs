#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(std140) uniform GlobalProjMatrices
{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;


out f2v
{
    vec3 worldPos;
    vec2 uvPos;
} o;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
}