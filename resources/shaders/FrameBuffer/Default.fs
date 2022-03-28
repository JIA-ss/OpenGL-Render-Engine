#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D colorAttachmentTex0;

//const float offset = 1.0 / 300.0;  

void main()
{

    vec2 texelSize = 1.0 / textureSize(colorAttachmentTex0, 0);

    vec2 offsets[9] = vec2[](
        texelSize * vec2(-1,1),     // 左上
        texelSize * vec2(0,1),  // 正上
        texelSize * vec2(1,1),  // 右上
        texelSize * vec2(-1,0), // 左
        texelSize * vec2(0,0),  // 中
        texelSize * vec2(1,0),  // 右
        texelSize * vec2(-1,-1),    // 左下
        texelSize * vec2(0,-1), // 正下
        texelSize * vec2(1,-1) // 右下
        
        //vec2(-offset,  offset), // 左上
        //vec2( 0.0f,    offset), // 正上
        //vec2( offset,  offset), // 右上
        //vec2(-offset,  0.0f),   // 左
        //vec2( 0.0f,    0.0f),   // 中
        //vec2( offset,  0.0f),   // 右
        //vec2(-offset, -offset), // 左下
        //vec2( 0.0f,   -offset), // 正下
        //vec2( offset, -offset)  // 右下
    );

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(colorAttachmentTex0, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}