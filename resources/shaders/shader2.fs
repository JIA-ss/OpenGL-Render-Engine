#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
uniform vec4 myColor;
in vec4 myPos;
void main()
{
    FragColor = myPos;
    //FragColor = myColor + vec4(ourColor,1.);
}