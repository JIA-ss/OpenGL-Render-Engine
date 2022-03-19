#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthAttachmentTex0;

//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // Back to NDC 
//    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
//}

void main()
{
    //FragColor = vec4(0.2,0.8,0.1,1.0);
    //FragColor = texture(depthAttachmentTex0, TexCoords);
    FragColor = vec4(vec3(texture(depthAttachmentTex0, TexCoords).r), 1.0);
}