#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float percent;

void main()
{
     //FragColor = texture(diffuseTex0, TexCoord);
     FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(TexCoord.x, TexCoord.y)), 0.5f) * vec4(ourColor, 1.0f); 
}