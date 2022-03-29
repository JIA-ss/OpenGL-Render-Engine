
#version 330 core
out vec4 FragColor;

uniform sampler2D colorAttachmentTex0;
uniform sampler2D colorAttachmentTex1;
uniform sampler2D colorAttachmentTex2;

in vec2 TexCoords;
void main()
{
    //FragColor = vec4(0,0,0,1);
    FragColor = vec4(texture(colorAttachmentTex2, TexCoords).r * 10,0,0,1);
}