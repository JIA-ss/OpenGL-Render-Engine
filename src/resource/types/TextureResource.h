#pragma once
#include "resource/resourceBasic.h"

namespace Resource
{

class TextureResource : public BaseResource
{
private:
    std::string m_name;
    std::string m_path;
    int m_width = 0, m_height = 0, m_channels = 0;
public:
    const std::string& getName() const { return m_name; }
    const std::string& getPath() const { return m_path; }
    virtual void loadFromPath(const char* path) override;
    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }
    inline int getChannels() const { return m_channels; }
    const unsigned char* getTextureData() const { return m_rawData.buffer(); }
    ~TextureResource();
};

typedef tResourceRef<TextureResource> TextureRef;

}