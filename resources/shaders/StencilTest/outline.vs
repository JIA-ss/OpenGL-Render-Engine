#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;


layout(std140) uniform GlobalProjMatrices
{
    mat4 view;
    mat4 projection;
};

layout(std140) uniform GlobalPositions
{
    vec3 camPos;
    vec3 lightPos;
};

layout(std140) uniform GlobalColors
{
    vec3 ambientColor;
    vec3 lightColor;
};

uniform int drawOutline;
uniform float outlineWidth;
uniform vec3 outlineColor;

uniform mat4 model;

out pos
{
    vec3 camera;
    vec3 light;
    vec3 frag;
    vec3 normal;
    vec2 uv;
}o_pos;

out color
{
    vec3 ambient;
    vec3 light;
}o_color;

out outline
{
    int use;
    float width;
    vec3 color;
}o_outline;

out float useOutline;

void main()
{
    o_pos.normal = normalize( mat3(transpose(inverse(model))) * normal );
    o_pos.camera = camPos;
    o_pos.light = lightPos;
    o_pos.uv = texCoord;

    o_color.ambient = ambientColor;
    o_color.light = lightColor;
    
    o_pos.frag = vec3(model * vec4(position, 1.0));
    if (drawOutline == 1)
    {
        o_pos.frag = o_pos.frag + o_pos.normal * outlineWidth;
        o_outline.use = 1;
        useOutline = 1;
    }
    else
    {
        o_outline.use = 0;
        useOutline = 0;
    }

    o_outline.width = outlineWidth;
    o_outline.color = outlineColor;
    gl_Position = projection * view * vec4(o_pos.frag, 1.0);
}