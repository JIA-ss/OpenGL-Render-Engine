#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec4 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 lightPos;
    vec3 viewPos;
    vec2 lightNearFar;
} fs_in;

uniform sampler2D depthAttachmentTex0;
uniform sampler2D diffuseTex0;
uniform sampler2D specularTex0;

float ClosestDepth()
{
    vec4 fragPosLightSpace = fs_in.FragPosLightSpace;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    return texture(depthAttachmentTex0, projCoords.xy).r; 
}

float CurrentDepth()
{
    return length(vec3(fs_in.FragPos) - fs_in.lightPos) / fs_in.lightNearFar.y;
}


float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{

    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    float currentDepth = length(vec3(fs_in.FragPos) - fs_in.lightPos) / fs_in.lightNearFar.y;

    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(depthAttachmentTex0, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度


    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthAttachmentTex0, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthAttachmentTex0, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(currentDepth > 1.0)
        shadow = 0;
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTex0, fs_in.TexCoords).rgb;
    vec3 specColor = texture(diffuseTex0, fs_in.TexCoords).rgb;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // Ambient
    vec3 ambient = 0.05 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.lightPos - fs_in.FragPos.xyz);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * specColor * lightColor;    
    // 计算阴影
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    

    vec3 _lighting_ = ambient + (1.0 - shadow) * (diffuse + specular);

    FragColor = vec4(_lighting_, 1.0f);
    //float closestDepth = ClosestDepth();
    //float currentDepth = CurrentDepth();
    //FragColor = vec4(currentDepth,currentDepth,currentDepth, 1.0);
}