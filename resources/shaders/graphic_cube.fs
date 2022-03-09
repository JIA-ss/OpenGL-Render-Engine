#version 330 core
out vec4 FragColor;

in vec2 uvPos;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture(texture0, uvPos), texture(texture1, uvPos), 0.2);
}