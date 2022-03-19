#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoord;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
}