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

float AverageBlockerDepth(float curDepth)
{
    vec4 fragPosLightSpace = fs_in.FragPosLightSpace;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;

    vec2 texelSize = 1.0 / textureSize(depthAttachmentTex0, 0);

    float res = 0;
    int blocker = 0;
    int totalNum = 0;
    for (int x = -9; x <= 9; x++)
    {
        for (int y = -9; y <= 9; y++)
        {
            float depth = texture(depthAttachmentTex0, projCoords.xy + vec2(x,y) * texelSize).r;
            if (depth < curDepth)
            {
                res += depth;
                blocker++;
            }
            totalNum++;
        }
    }

    if (blocker == 0)
        return -1;
    if (blocker == totalNum)
        return -2;

    return res / blocker ; 
}

float PCF(float currentDepth, float penumbraScale, float bias)
{
    vec4 fragPosLightSpace = fs_in.FragPosLightSpace;
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;

    vec2 texelSize = 1.0 / textureSize(depthAttachmentTex0, 0);

    float range = 3 + 30 * penumbraScale;
    highp int index = int(range);

    int num = 0;
    float visible = 0;
    for(int x = -index / 2; x <= index / 2; ++x)
    {
        for(int y = -index / 2; y <= index / 2; ++y)
        {
            float pcfDepth = texture(depthAttachmentTex0, projCoords.xy + vec2(x, y) * texelSize).r; 
            visible += currentDepth - bias < pcfDepth ? 1.0 : 0.0;        
            num ++;
        }    
    }
    return visible / num;
}

vec3 blinnPhone(vec3 color)
{
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
    vec3 specular = spec * lightColor;    

    return ambient + diffuse + specular;
    //float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);       
}

float _CURRENT_DEPTH_ = 0;

float _AVERAGE_DEPTH_ = 1;

float _PENUMBRA_SCALE = 0;

float _BIAS_ = 0;

float pcssVisible()
{
    _CURRENT_DEPTH_ = length(vec3(fs_in.FragPos) - fs_in.lightPos) / fs_in.lightNearFar.y;

    // STEP1: blocker search
    _AVERAGE_DEPTH_ = AverageBlockerDepth(_CURRENT_DEPTH_);

    if (_AVERAGE_DEPTH_ == -1 )
    {
        _AVERAGE_DEPTH_ = 1;
        return 1;
    }
    if (_AVERAGE_DEPTH_ == -2)
    {
        _AVERAGE_DEPTH_ = 0;
        return 0;
    }
    
    // STEP2: penumbra size
    _PENUMBRA_SCALE =(_CURRENT_DEPTH_ - _AVERAGE_DEPTH_) / _AVERAGE_DEPTH_;

    // STEP3: PCF
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(fs_in.lightPos - fs_in.FragPos.xyz);
    _BIAS_ = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    return PCF(_CURRENT_DEPTH_, _PENUMBRA_SCALE, _BIAS_);
}

void main()
{           
    vec3 color = texture(diffuseTex0, fs_in.TexCoords).rgb;
    vec3 lighting = blinnPhone(color);

    float visible = pcssVisible();

    if (visible > 0.1)
    {
        lighting *= visible;
    }
    else
    {
        lighting = 0.05 * color;
    }

    FragColor = vec4(lighting, 1.0f);
}