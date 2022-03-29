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

layout(std140) uniform GlobalColors
{
    vec3 ambientColor;
    vec3 lightColor;
};

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

void main()
{
    o_pos.frag = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * vec4(o_pos.frag, 1.0);

    o_pos.normal = mat3(transpose(inverse(model))) * normal;
    o_pos.camera = camPos;
    o_pos.light = lightPos;
    o_pos.uv = texCoord;

    o_color.ambient = ambientColor;
    o_color.light = vec3(1.0);
}