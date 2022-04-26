#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D diffuseTex0;

void main()
{
    FragColor = texture(diffuseTex0, TexCoord);
    //FragColor = vec4(1,0,1,1);
}