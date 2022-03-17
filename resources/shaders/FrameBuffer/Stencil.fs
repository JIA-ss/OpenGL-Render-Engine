#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D stencilAttachmentTex0;

void main()
{
    FragColor = texture(stencilAttachmentTex0, TexCoords);
}