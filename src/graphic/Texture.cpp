#include "Texture.h"
#include "system/ResourceSystem.h"
GRAPHIC_NAMESPACE_USING


std::unordered_map<std::string, Texture> Texture::collection = std::unordered_map<std::string, Texture>();

Texture::Texture(const std::string& texName,            
            const InternalFormat &internalFormat,
            const DataFormat &format,
            const DataType &type)
{
    m_textureRef = ResourceSystem::Get()->GetResource(texName.c_str(), eResourceType::texture);
    const ubyte* data = m_textureRef->getTextureData();
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D,0, internalFormat, GetWidth(), GetHeight(), 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    std::cout << "gen tex: " << GetName() << "\ttype: " << Texture::ToString(m_type) << std::endl;
}

Texture::Texture(const TextureRef& tex,            
            const InternalFormat &internalFormat,
            const DataFormat &format,
            const DataType &type)
{
    m_textureRef = tex;
    const ubyte* data = m_textureRef->getTextureData();
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D,0, internalFormat, GetWidth(), GetHeight(), 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    std::cout << "gen tex: " << GetName() << "\ttype: " << Texture::ToString(m_type) << std::endl;

}

Texture::Texture(const std::string& texName, const TextureType& textureType)
{
    std::cout << "gen tex: " << texName << "\ttype: " << Texture::ToString(textureType) << std::endl;
    m_textureRef = ResourceSystem::Get()->GetResource(texName.c_str(), eResourceType::texture);
    m_type = textureType;
    const ubyte* data = m_textureRef->getTextureData();
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D,0, InternalFormat::RGBAInternal, GetWidth(), GetHeight(), 0, DataFormat::RGBA, DataType::UnsignedByte, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
Texture::Texture(const TextureRef& tex, const TextureType& textureType)
{
    m_textureRef = tex;
    m_type = textureType;
    const ubyte* data = m_textureRef->getTextureData();
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D,0, InternalFormat::RGBAInternal, GetWidth(), GetHeight(), 0, DataFormat::RGBA, DataType::UnsignedByte, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    std::cout << "gen tex: " << GetName() << "\ttype: " << Texture::ToString(textureType) << std::endl;
}

Texture::Texture(const TextureType& textureType,             
            const int& width, const int& height,
            const InternalFormat &internalFormat,
            const DataFormat &format,
            const DataType &type)
{
    m_type = textureType;
    m_height = height;
    m_width = width;
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout << "gen tex: " << GetName() << "\ttype: " << Texture::ToString(textureType) << std::endl;
}

void Texture::Free()
{
    glDeleteTextures(1, &m_textureId);
    m_textureId = 0;
}

int Texture::GetWidth() const
{
    return !m_textureRef.isNull() ? m_textureRef.get()->getWidth() : m_width;
}

int Texture::GetHeight() const 
{
    return !m_textureRef.isNull() ? m_textureRef.get()->getHeight() : m_height;
}

const unsigned int& Texture::GetId() const
{
    return m_textureId;
}

const std::string& Texture::GetName() const
{
    return !m_textureRef.isNull() ? m_textureRef.get()->getName() : ToString(m_type);
}

int Texture::GetNRChannel() const
{
    return m_textureRef.get()->getChannels();
}

const TextureType& Texture::GetType() const
{
    return m_type;
}

Texture* Texture::Get(const std::string& id)
{
    auto iter = collection.find(id);
    if (iter == collection.end())
        return nullptr;
    return &iter->second;
}
void Texture::Remove(const std::string& id)
{
    collection.erase(id);
}

#define TEXTURE_TYPE_TO_STRING(type)    \
    case type:                          \
        return #type;                   

std::string Texture::ToString(TextureType type)
{
    switch (type)
    {
        TEXTURE_TYPE_TO_STRING(None)
        TEXTURE_TYPE_TO_STRING(Diffuse)
        TEXTURE_TYPE_TO_STRING(Specular)
        TEXTURE_TYPE_TO_STRING(Ambient)
        TEXTURE_TYPE_TO_STRING(Emissive)
        TEXTURE_TYPE_TO_STRING(Height)
        TEXTURE_TYPE_TO_STRING(Normals)
        TEXTURE_TYPE_TO_STRING(Shininess)
        TEXTURE_TYPE_TO_STRING(Opacity)
        TEXTURE_TYPE_TO_STRING(Displacement)
        TEXTURE_TYPE_TO_STRING(Lightmap)
        TEXTURE_TYPE_TO_STRING(Reflection)
        TEXTURE_TYPE_TO_STRING(Unknown)
        TEXTURE_TYPE_TO_STRING(ColorAttachment)
        TEXTURE_TYPE_TO_STRING(DepthAttachment)
        TEXTURE_TYPE_TO_STRING(StencilAttachment)
    default:
        return "";
    }
    return "";
}