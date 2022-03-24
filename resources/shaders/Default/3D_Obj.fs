#version 330 core
out vec4 FragColor;

in pos
{
    vec3 camera;
    vec3 light;
    vec3 frag;
    vec3 normal;
    vec2 uv;
}i_pos;

in color
{
    vec3 ambient;
    vec3 light;
}i_color;

uniform vec3 objectColor;
uniform sampler2D diffuseTex0;

void main()
{
    FragColor = texture(diffuseTex0, i_pos.uv);
}