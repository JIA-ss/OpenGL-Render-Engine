#pragma once
#include "Texture.h"
#include <vector>
#include <string>
#include <map>
GRAPHIC_NAMESPACE_BEGIN

class CubeMap : public Texture
{
public:
    enum Target
    {
        Right  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,     //	右
        Left   = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,     //	左
        Top    = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,     //	上
        Bottom = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,     //	下
        Front   = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,     //	后
        Back  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,     //	前
    };
    std::string to_string(Target);

    CubeMap(const std::string& groupPath, const std::string& suffix);
    CubeMap(const std::vector<std::string>& facesPath);
    CubeMap(const std::map<Target, std::string>& facesPath);
    ~CubeMap() { Texture::Free(); }

    void SetUp();
    Resource::TextureRef GetTargetRef(Target);
    void SetTargetRef(Target, Resource::TextureRef);
private:
    std::map<Target, Resource::TextureRef> m_textures;
};

GRAPHIC_NAMESPACE_END