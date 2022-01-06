#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色
out vec4 myPos;
uniform float xOffSet;

void main()
{
    gl_Position = vec4(aPos.x + xOffSet, -aPos.y, aPos.z, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
    myPos = gl_Position;
}