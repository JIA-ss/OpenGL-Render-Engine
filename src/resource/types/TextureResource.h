#pragma once
#include "resource/resourceBasic.h"
#include "common/headers.h"
namespace Resource
{

class TextureResource : public BaseResource
{
protected:
    std::string m_name;
    std::string m_path;
    int m_width = 0, m_height = 0, m_channels = 0;
public:
    const std::string& getName() const { return m_name; }
    const std::string& getPath() const { return m_path; }
    virtual bool loadFromPath(const char* path) override;
    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }
    inline int getChannels() const { return m_channels; }
    const unsigned char* getTextureData() const { return m_rawData.buffer(); }
    GLint getFormat() const { return m_channels == 4 ? GL_RGBA : GL_RGB; }
    ~TextureResource();
};

typedef tResourceRef<TextureResource> TextureRef;

}