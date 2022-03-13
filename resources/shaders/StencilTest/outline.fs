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

in outline
{
    float use;
    float width;
    vec3 color;
}i_outline;

in float useOutline;

uniform sampler2D diffuseTex0;
uniform sampler2D specularTex0;

void main()
{
    vec3 objectColor = vec3(1,1,1);
    vec3 diffuseTex = vec3(texture(diffuseTex0, i_pos.uv));
    vec3 specularTex = vec3(texture(specularTex0, i_pos.uv));
    // ambient
    vec3 ambient = 0.8 * i_color.light * diffuseTex;

    //diffuse
    vec3 normal = normalize(i_pos.normal);
    vec3 lightDir = normalize(i_pos.light - i_pos.frag);
    float diffuseCoeffi = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseCoeffi * i_color.light * diffuseTex;

    //specular
    float specularCoeffi = 0.5;
    vec3 viewDir = normalize(i_pos.camera - i_pos.frag);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularCoeffi * spec * i_color.light * specularTex;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    //result = diffuseTex;
    if (useOutline == 1)
    {
        //FragColor = vec4(1,1,1,1);
        FragColor = vec4(i_outline.color, 1);
    }
    else
    {
        FragColor = vec4(result, 1);
    }
}