#pragma once
#include "GraphicCommonHeader.h"
#include "resource/types/TextureResource.h"
#include <assimp/material.h>
#include <unordered_map>
GRAPHIC_NAMESPACE_BEGIN
using namespace Resource;


enum TextureType
{
    None = aiTextureType_NONE,
    Diffuse = aiTextureType_DIFFUSE,
    Specular = aiTextureType_SPECULAR,
    Ambient = aiTextureType_AMBIENT,
    Emissive = aiTextureType_EMISSIVE,
    Height = aiTextureType_HEIGHT,
    Normals = aiTextureType_NORMALS,
    Shininess = aiTextureType_SHININESS,
    Opacity = aiTextureType_OPACITY,
    Displacement = aiTextureType_DISPLACEMENT,
    Lightmap = aiTextureType_LIGHTMAP,
    Reflection = aiTextureType_REFLECTION,
    Unknown = aiTextureType_UNKNOWN,
};

class Texture
{
public:
	enum InternalFormat
    {
        RGBAInternal = GL_RGBA,
        RGBInternal = GL_RGB,
        RGB8 = GL_RGB8,
        RGBA8 = GL_RGBA8,
        RGBA16F = GL_RGBA16F,
        RGB16F = GL_RGB16F,
        DepthComponent = GL_DEPTH_COMPONENT,
        DepthComponent16 = GL_DEPTH_COMPONENT16,
        DepthComponent24 = GL_DEPTH_COMPONENT24,
        DepthComponent32 = GL_DEPTH_COMPONENT32,
        StencilIndex = GL_STENCIL_INDEX,
        StencilIndex1 = GL_STENCIL_INDEX1,
        StencilIndex4 = GL_STENCIL_INDEX4,
        StencilIndex8 = GL_STENCIL_INDEX8,
        StencilIndex16 = GL_STENCIL_INDEX16,
        DepthStencilInternal = GL_DEPTH_STENCIL
    };
    enum DataFormat
    {
        Red = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        BGR = GL_BGR,
        RGBA = GL_RGBA,
        BGRA = GL_BGRA,
        RedInteger  =GL_RED_INTEGER,
        RGInteger = GL_RG_INTEGER,
        RGBInteger = GL_RGB_INTEGER,
        BGRInteger = GL_BGR_INTEGER,
        RGBAInteger = GL_RGBA_INTEGER,
        BGRAInteger = GL_BGRA_INTEGER,
        Stencil = GL_STENCIL_INDEX,
        Depth = GL_DEPTH_COMPONENT,
        DepthStencil = GL_DEPTH_STENCIL
    };
    enum DataType
    {
        UnsignedByte = GL_UNSIGNED_BYTE,
        Byte = GL_BYTE,
        UnsignedShort = GL_UNSIGNED_SHORT,
        Short = GL_SHORT,
        UnsignedInt = GL_UNSIGNED_INT,
        Int = GL_INT,
        HalfFloat = GL_HALF_FLOAT,
        Float = GL_FLOAT,
        UnsignedByte332 = GL_UNSIGNED_BYTE_3_3_2
    };

public:
    Texture() = default;
    Texture(const std::string& texName,            
            const InternalFormat &internalFormat,
            const DataFormat &format,
            const DataType &type);

    Texture(const TextureRef& tex,            
            const InternalFormat &internalFormat,
            const DataFormat &format,
            const DataType &type);

    Texture(const std::string& texName, const TextureType& textureType = TextureType::Diffuse);
    Texture(const TextureRef& tex, const TextureType& textureType = TextureType::Diffuse);
    
    //todo: freeType Texture
    //Texture(FT_GlyphSlot glyph);
public:
    const unsigned int &GetId() const;
    const std::string &GetName() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetNRChannel() const;
    const TextureType &GetType() const;
    void Free();

private:
    unsigned int m_textureId = 0;
    TextureRef m_textureRef;
    TextureType m_type = TextureType::None;

public:
    static Texture* Get(const std::string& id);
    static void Remove(const std::string& id);
    
    template<typename... Args>
    static Texture* Add(const std::string& id, Args &&... args);

private:
    static std::unordered_map<std::string, Texture> collection;
};

template<typename... Args>
Texture* Texture::Add(const std::string& id, Args &&... args)
{
    auto it = collection.find(id);
    if (it == collection.end())
    {
        auto pair = collection.emplace(id, Texture(args...));
        return &pair.first->second;
    }
    return &it->second;
}

GRAPHIC_NAMESPACE_END