
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
    mat4 lightSpaceMatrice
};

uniform mat4 model;


void main()
{
    gl_Position = lightSpaceMatrice * model * vec4(position, 1.0f);
}