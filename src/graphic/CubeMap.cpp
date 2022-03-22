#include "CubeMap.h"
#include "system/ResourceSystem.h"
GRAPHIC_NAMESPACE_USING

#define CUBEMAP_TARGET_TO_STRING(_TARGET_)  \
    case _TARGET_:                          \
        return #_TARGET_;

std::string CubeMap::to_string(Target target)
{
    switch (target)
    {
    CUBEMAP_TARGET_TO_STRING(Right)
    CUBEMAP_TARGET_TO_STRING(Left)
    CUBEMAP_TARGET_TO_STRING(Top)
    CUBEMAP_TARGET_TO_STRING(Bottom)
    CUBEMAP_TARGET_TO_STRING(Front)
    CUBEMAP_TARGET_TO_STRING(Back)
    default:
        break;
    }
    return "Error";
}

CubeMap::CubeMap(const std::string& groupPath, const std::string& suffix)
{
    for (GLuint tar = (GLuint)Right; tar <= (GLuint)Back; tar++)
    {
        std::string path = groupPath + to_string((Target)tar) + suffix;
        m_textures[(Target)tar] = ResourceSystem::Get()->GetResource(path.c_str(), eResourceType::texture);
    }
    SetUp();
    
}

CubeMap::CubeMap(const std::vector<std::string>& facesPath)
{
    assert(facesPath.size() == (unsigned int)(Front - Back));

    for (GLuint tar = (GLuint)Right; tar <= (GLuint)Back; tar++)
    {
        std::string path = facesPath[tar - (GLuint)Right];
        m_textures[(Target)tar] = ResourceSystem::Get()->GetResource(path.c_str(), eResourceType::texture);
    }
    SetUp();
}

CubeMap::CubeMap(const std::map<Target, std::string>& facesPath)
{
    assert(facesPath.size() == (unsigned int)(Front - Back));
    for (auto&&[target, path] : facesPath)
    {
        m_textures[target] = ResourceSystem::Get()->GetResource(path.c_str(), eResourceType::texture);
    }
    SetUp();
}

void CubeMap::SetUp()
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    for (auto&&[target, ref] : m_textures)
    {
        if (ref.isNull())
        {
            std::cout << "CubeMap SetUp " << to_string(target) << " Error" << std::endl; 
            continue;
        }
        glTexImage2D(
            (GLenum)target, 0, GL_RGBA, ref->getWidth(), ref->getHeight(), 0, GL_RGBA, 
            GL_UNSIGNED_BYTE, ref->getTextureData()
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_type = TextureType::CubeMapType;
}

Resource::TextureRef CubeMap::GetTargetRef(Target tar)
{
    return m_textures[tar];
}

void CubeMap::SetTargetRef(Target tar, Resource::TextureRef ref)
{
    if (ref == m_textures[tar])
    {
        return;
    }
    m_textures[tar] = ref;
}