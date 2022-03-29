
#version 330 core


layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuseTex0;
uniform sampler2D specularTex0;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = fs_in.FragPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(fs_in.Normal);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(diffuseTex0, fs_in.TexCoords).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(specularTex0, fs_in.TexCoords).r;
}