
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

layout(std140) uniform LightPositions
{
    vec3 lightPos0;
    vec3 lightPos1;
    vec3 lightPos2;
    vec3 lightPos3;
    vec3 lightPos4;
};

layout(std140) uniform LightColors
{
    vec3 lightColor0;
    vec3 lightColor1;
    vec3 lightColor2;
    vec3 lightColor3;
    vec3 lightColor4;
};

uniform mat4 model;


out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;


void main()
{
    vec4 worldPos = model * vec4(position, 1.0f);
    gl_Position = projection * view * worldPos;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.FragPos = worldPos.xyz; 
    vs_out.TexCoords = texCoord;
    vs_out.Normal = normalMatrix * normal;
}