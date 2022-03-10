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
    // ambient
    vec3 ambient = 0.2 * i_color.light;

    //diffuse
    vec3 normal = normalize(i_pos.normal);
    vec3 lightDir = normalize(i_pos.light - i_pos.frag);
    float diffuseCoeffi = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseCoeffi * i_color.light;

    //specular
    float specularCoeffi = 0.5;
    vec3 viewDir = normalize(i_pos.camera - i_pos.frag);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularCoeffi * spec * i_color.light;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1);
    //FragColor = vec4(result, 1.0);
}