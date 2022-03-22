#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubeMapTex0;

void main()
{    
    FragColor = texture(cubeMapTex0, TexCoords);
}