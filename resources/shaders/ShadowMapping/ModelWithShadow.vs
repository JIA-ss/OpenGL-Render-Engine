
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


out VS_OUT {
    vec4 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 lightPos;
    vec3 viewPos;
    vec2 lightNearFar;
} vs_out;


void main()
{
    vec4 fragPos = model * vec4(position, 1.0);
    gl_Position = projection * view * fragPos;
    vs_out.FragPos = fragPos;
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoords = texCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrice * fragPos;
    vs_out.lightPos = lightPos;
    vs_out.viewPos = camPos;
    vs_out.lightNearFar = lightNearFar;
}