#include "ShaderSetting.h"

#include "system/RenderSystem.h"

#include <iostream>
GRAPHIC_NAMESPACE_USING

void ShaderSetting::Use() const
{
    for (auto&& [name, param] : m_params)
    {
        //std::cout << name << "\tuse" << std::endl;
        param->Use();
    }
}

ShaderSetting::ShaderParamBase::ShaderParamBase(const unsigned int &loc, const ShaderParamType &type)
    : m_loc(loc), m_type(type) { }

void ShaderSetting::ShaderParamBase::updateLocation(const unsigned int& loc)
{
    m_loc = loc;
}

ShaderParamType ShaderSetting::ShaderParamBase::GetType() const 
{
    return m_type;
}


void ShaderSetting::SetTextures(const std::vector<Texture*>& newTextures)
{
    static const std::string NoneTex = "texture";
    static const std::string DiffuseTex = "diffuseTex";
    static const std::string SpecularTex = "specularTex";
    static const std::string AmbientTex = "ambientTex";
    static const std::string EmissiveTex = "emissiveTex";
    static const std::string HeightTex = "heightTex";
    static const std::string NormalsTex = "normalsTex";
    static const std::string ShininessTex = "shininessTex";
    static const std::string OpacityTex = "opacityTex";
    static const std::string DisplacementTex = "displacementTex";
    static const std::string LightmapTex = "lightmapTex";
    static const std::string ReflectionTex = "reflectionTex";
    static const std::string ColorAttachTex = "colorAttachmentTex";
    static const std::string DepthAttachTex = "depthAttachmentTex";
    static const std::string StencilAttachTex = "stencilAttachmentTex";
    static const std::string CubeMapTex = "cubeMapTex";

    int noneCount = 0;
    int diffuseCount = 0;
    int specularCount = 0;
    int ambientCount = 0;
    int emissiveCount = 0;
    int heightCount = 0;
    int normalsCount = 0;
    int shininessCount = 0;
    int opacityCount = 0;
    int displacementCount = 0;
    int lightmapCount = 0;
    int reflectionCount = 0;
    int colorAttachCount = 0;
    int cubemapCount = 0;

    size_t texUnit = 0;
    for (texUnit; texUnit < newTextures.size(); texUnit++)
    {
        const std::string *textureName;
        int count;
        switch (newTextures[texUnit]->GetType())
        {
        case None:
            count = noneCount++;
            textureName = &NoneTex;
            break;
        case Diffuse:
            count = diffuseCount++;
            textureName = &DiffuseTex;
            break;
        case Specular:
            count = specularCount++;
            textureName = &SpecularTex;
            break;
        case Ambient:
            count = ambientCount++;
            textureName = &AmbientTex;
            break;
        case Emissive:
            count = emissiveCount++;
            textureName = &EmissiveTex;
            break;
        case Height:
            count = heightCount++;
            textureName = &HeightTex;
            break;
        case Normals:
            count = normalsCount++;
            textureName = &NormalsTex;
            break;
        case Shininess:
            count = shininessCount++;
            textureName = &ShininessTex;
            break;
        case Opacity:
            count = opacityCount++;
            textureName = &OpacityTex;
            break;
        case Displacement:
            count = displacementCount++;
            textureName = &DisplacementTex;
            break;
        case Lightmap:
            count = lightmapCount++;
            textureName = &LightmapTex;
            break;
        case Reflection:
            count = reflectionCount++;
            textureName = &ReflectionTex;
            break;
        case ColorAttachment:
            count = colorAttachCount++;
            textureName = &ColorAttachTex;
            break;
        case DepthAttachment:
            count = 0;
            textureName = &DepthAttachTex;
            break;
        case StencilAttachment:
            count = 0;
            textureName = &StencilAttachTex;
            break;
        case CubeMapType:
            count = cubemapCount++;
            textureName = &CubeMapTex;
            break;
        default:
            std::cerr << "\nUNSUPPORTED texture type:" << newTextures[texUnit]->GetName();
            return;
        }
        SetParameter(*textureName + std::to_string(count),
                     TextureParamValue(texUnit, newTextures[texUnit]));
    }

    Render::RenderPath curRenderPath = RenderSystem::Get()->GetCurRenderPath();
    switch (curRenderPath)
    {
    case Render::Forward:
    {
        break;
    }
    case Render::Deferred:
    {
        auto& deferredRender = RenderSystem::Get()->GetDeferredRendering();
        SetParameter("gPosAttachment", TextureParamValue(texUnit++, deferredRender.GetPosTexture()));
        SetParameter("gNormalAttachment", TextureParamValue(texUnit++, deferredRender.GetNormalTexture()));
        SetParameter("gColorAttachment", TextureParamValue(texUnit++, deferredRender.GetColorTexture()));
        break;
    }
    default:
        break;
    }
}

template <>
void ShaderSetting::ShaderParam<ShaderSetting::TextureParamValue>::Use()
{
    glActiveTexture(GL_TEXTURE0 + m_val.texUnit); 
    if(m_val.texture)
    {
        GLenum textureType = m_val.texture->GetType() == CubeMapType ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
        glBindTexture(textureType, m_val.texture->GetId());
    }
    else 
        glBindTexture(GL_TEXTURE_2D, 0);
    Shader::setUniform(m_loc, m_val.texUnit);
}



#define MAKE_CASE(glType, storeType, enumType, glFunc)                                                                  \
    case glType:                                                                                                        \
        for (int s = 0; s < size; s++)                                                                                  \
        {                                                                                                               \
            if (s > 0)                                                                                                  \
            {                                                                                                           \
                nameStr = nameStr.substr(0, nameStr.find_last_of("["));                                                 \
                nameStr += "[" + std::to_string(s) + "]";                                                               \
            }                                                                                                           \
            auto loc = glGetUniformLocation(program, nameStr.data());                                                   \
            if (loc != GL_INVALID_INDEX)                                                                                \
            {                                                                                                           \
                storeType val;                                                                                          \
                glFunc(program, loc, &val);                                                                             \
                m_params.emplace(nameStr, new ShaderParam<storeType>(val, loc, ShaderParamType::enumType));             \
            }                                                                                                           \
        }                                                                                                               \
        break;  
    
#define MAKE_CASE_VM(glType, storeType, enumType, glFunc)                                                               \
    case glType:                                                                                                        \
        for (int s = 0; s < size; s++)                                                                                  \
        {                                                                                                               \
            if (s > 0)                                                                                                  \
            {                                                                                                           \
                nameStr = nameStr.substr(0, nameStr.find_last_of("["));                                                 \
                nameStr += "[" + std::to_string(s) + "]";                                                               \
            }                                                                                                           \
            auto loc = glGetUniformLocation(program, nameStr.data());                                                   \
            if (loc != GL_INVALID_INDEX)                                                                                \
            {                                                                                                           \
                storeType val;                                                                                          \
                glFunc(program, loc, glm::value_ptr(val));                                                              \
                m_params.emplace(nameStr, new ShaderParam<storeType>(val, loc, ShaderParamType::enumType));             \
            }                                                                                                           \
        }                                                                                                               \
        break;

#define MAKE_CASE_TEXTURE(glType, enumType, glFunc)                                                                     \
    case glType:                                                                                                        \
        for (int s = 0; s < size; s++)                                                                                  \
        {                                                                                                               \
            if (s > 0)                                                                                                  \
            {                                                                                                           \
                nameStr = nameStr.substr(0, nameStr.find_last_of("["));                                                 \
                nameStr += "[" + std::to_string(s) + "]";                                                               \
            }                                                                                                           \
            auto loc = glGetUniformLocation(program, nameStr.data());                                                   \
            if (loc != GL_INVALID_INDEX)                                                                                \
            {                                                                                                           \
                GLint val;                                                                                              \
                glFunc(program, loc, &val);                                                                             \
                TextureParamValue actualVal(loc, nullptr);                                                              \
                m_params.emplace(nameStr, new ShaderParam<TextureParamValue>(actualVal, loc, ShaderParamType::enumType));   \
            }                                                                                                           \
        }                                                                                                               \
        break;


void ShaderSetting::UpdateParameters(Shader* shader)
{
    Render::RenderPath curRenderPath = RenderSystem::Get()->GetCurRenderPath();
    m_params.clear();
    auto program = shader->ID;
    const GLsizei buffSize = 256;
    GLint count;
    GLchar name[buffSize];
    GLsizei length;
    GLenum type;
    GLint size;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++)
    {
        glGetActiveUniform(program, (GLuint)i, buffSize, &length, &size, &type, name);
        std::string nameStr = std::string(name);
        switch (type)
        {
            MAKE_CASE(GL_FLOAT, float, Float, glGetUniformfv);
            MAKE_CASE(GL_DOUBLE, double, Double, glGetUniformdv);
            MAKE_CASE(GL_INT, int, Int, glGetUniformiv);
            MAKE_CASE(GL_UNSIGNED_INT, unsigned int, UInt, glGetUniformuiv);
            MAKE_CASE(GL_BOOL, int, Bool, glGetUniformiv);
            MAKE_CASE_VM(GL_FLOAT_VEC2, glm::vec2, Vec2, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_VEC3, glm::vec3, Vec3, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_VEC4, glm::vec4, Vec4, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_MAT2, glm::mat2, Mat2, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_MAT3, glm::mat3, Mat3, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_MAT4, glm::mat4, Mat4, glGetUniformfv);
            MAKE_CASE_TEXTURE(GL_SAMPLER_2D, Sampler2D, glGetUniformiv);
            MAKE_CASE_TEXTURE(GL_SAMPLER_CUBE, SamplerCube, glGetUniformiv);
        default:
            std::cerr << "type not supported in shader parameter: " << type << " " << name << std::endl;
            break;
        }
    }

    auto& deferredRender = RenderSystem::Get()->GetDeferredRendering();
    TextureParamValue* posAttach = GetParameter<TextureParamValue>("gPosAttachment");
    TextureParamValue* normalAttach = GetParameter<TextureParamValue>("gNormalAttachment");
    TextureParamValue* colorAttach = GetParameter<TextureParamValue>("gColorAttachment");
    if (posAttach)
        posAttach->texture = deferredRender.GetPosTexture();
    if (normalAttach)
        normalAttach->texture = deferredRender.GetNormalTexture();
    if (colorAttach)
        colorAttach->texture = deferredRender.GetColorTexture();
}

void* ShaderSetting::GetRawValue(const std::string& name)
{
    auto it = m_params.find(name);
    if (it == m_params.end())
        return nullptr;
    return it->second->Get();
}
    
void ShaderSetting::SetRawValue(const std::string& name, void* value)
{
    auto it = m_params.find(name);
    if (it == m_params.end())
        return;
    it->second->Set(value);
}


ShaderSetting::ShaderParamBase* ShaderSetting::GetRawParameter(const std::string& name)
{
    auto it = m_params.find(name);
    if (it == m_params.end())
        return nullptr;
    return it->second.get();
}

void ShaderSetting::Assign(ShaderSetting* ss) const
{
    for (auto it = m_params.begin(); it != m_params.end(); it++)
    {
        ShaderParamBase* param = ss->GetRawParameter(it->first);
        if (param != nullptr && param->GetType() == it->second->GetType())
        {
            ss->SetRawValue(it->first, it->second->Get());
        }
    }
}

ShaderSetting ShaderSetting::Clone() const
{
    ShaderSetting ss;
    for (auto it = m_params.begin(); it != m_params.end(); it++)
    {
        ss.m_params[it->first] = it->second->Clone();
    }
    return ss;
}
