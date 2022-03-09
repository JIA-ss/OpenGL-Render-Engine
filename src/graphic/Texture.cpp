#include "Texture.h"
#include "resource/resourceManager.h"
GRAPHIC_NAMESPACE_USING


std::unordered_map<std::string, Texture> Texture::collection = std::unordered_map<std::string, Texture>();

Texture::Texture(const std::string& texName,            
            const InternalFormat &internalFormat,
            const DataFormat &format,
            const DataType &type)
{
    m_textureRef = ResourceManager::Instance()->GetResource(texName.c_str(), eResourceType::texture);
    const ubyte* data = m_textureRef->getTextureData();
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D,0, internalFormat, GetWidth(), GetHeight(), 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

}

Texture::Texture(const std::string& texName, const TextureType& textureType)
{
    m_textureRef = ResourceManager::Instance()->GetResource(texName.c_str(), eResourceType::texture);
    m_type = textureType;
}
Texture::Texture(const TextureRef& tex, const TextureType& textureType)
{
    m_textureRef = tex;
    m_type = textureType;
}

void Texture::Free()
{
    glDeleteTextures(1, &m_textureId);
    m_textureId = 0;
}

int Texture::GetWidth() const
{
    return m_textureRef.get()->getWidth();
}

int Texture::GetHeight() const 
{
    return m_textureRef.get()->getHeight();
}

const unsigned int& Texture::GetId() const
{
    return m_textureId;
}

const std::string& Texture::GetName() const
{
    return m_textureRef.get()->getName();
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