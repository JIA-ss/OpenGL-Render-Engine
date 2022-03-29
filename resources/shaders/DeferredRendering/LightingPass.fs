#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosAttachment;  // pos
uniform sampler2D gNormalAttachment;  // normal
uniform sampler2D gColorAttachment;  // albedo spec

layout(std140) uniform LightPositions
{
    vec3 lightPos0;
    vec3 lightPos1;
    vec3 lightPos2;
    vec3 lightPos3;
    vec3 lightPos4;
};

layout(std140) uniform LightColors
{
    vec3 lightColor0;
    vec3 lightColor1;
    vec3 lightColor2;
    vec3 lightColor3;
    vec3 lightColor4;
};

layout(std140) uniform GlobalPositions
{
    vec3 camPos;
    vec3 lightPos;
    vec2 lightNearFar;
};


#define __LIGHT_POSITION__(i)    lightPos##i
#define __LIGHT_COLOR__(i)       lightColor##i

#define LIGHT_POS_CONVERT(i)    if (input == i) return __LIGHT_POSITION__(i);
#define LIGHT_COL_CONVERT(i)    if (input == i) return __LIGHT_COLOR__(i);
vec3 LIGHT_POSITION(int input)
{
    LIGHT_POS_CONVERT(0);
    LIGHT_POS_CONVERT(1);
    LIGHT_POS_CONVERT(2);
    LIGHT_POS_CONVERT(3);
    LIGHT_POS_CONVERT(4);
}
vec3 LIGHT_COLOR(int input)
{
    LIGHT_COL_CONVERT(0);
    LIGHT_COL_CONVERT(1);
    LIGHT_COL_CONVERT(2);
    LIGHT_COL_CONVERT(3);
    LIGHT_COL_CONVERT(4);
}

void main()
{             
    float Linear = 0.7;
    float Quadratic = 1.8;
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosAttachment, TexCoords).rgb;
    vec3 Normal = texture(gNormalAttachment, TexCoords).rgb;
    vec3 Diffuse = texture(gColorAttachment, TexCoords).rgb;
    float Specular = texture(gColorAttachment, TexCoords).a;
    
    // Then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(camPos - FragPos);
    for(int i = 0; i < 5; ++i)
    {
        // Diffuse
        vec3 lightDir = normalize(LIGHT_POSITION(i) - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * LIGHT_COLOR(i);
        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = LIGHT_COLOR(i) * spec * Specular;
        // Attenuation
        float distance = length(LIGHT_POSITION(i) - FragPos);
        float attenuation = 1.0 / (1.0 + Linear * distance + Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }    
    
    FragColor = vec4(lighting, 1.0);
}