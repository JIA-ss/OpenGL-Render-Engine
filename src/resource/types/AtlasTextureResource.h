#pragma once
#include "resource/resourceBasic.h"
#include "common/headers.h"
#include "resource/types/TextureResource.h"
#include <vector>
namespace Resource
{

enum class eAtlasSize
{
	kSize_256x256 = 1 << 8,
	kSize_512x512 = 1 << 9,
	kSize_1024x1024 = 1 << 10,
	kSize_2048x2048 = 1 << 11
};

struct sAtlasCell
{
	std::string name;
	std::string ref;
	unsigned int index;
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
};

struct sAtlasCellCtx
{
    sAtlasCell cell;
    TextureRef texture;
    bool packed;
};

class AtlasTextureResource
{
public:

    eAtlasSize getSize() const { return m_size; }
    unsigned int getRawSize() const { return static_cast<unsigned int>(m_size); }
    const unsigned char* getImageData() const { return m_buffer.get(); }

public:
    void setSize(eAtlasSize size);
    sAtlasCell addTexture(std::string_view name, TextureRef texture);
    bool pack();
    sAtlasCell getCell(std::string name)
    {
        for (int i = 0; i < m_cellCtxs.size(); ++i)
        {
            if (m_cellCtxs[i].cell.name == name)
            {
                return m_cellCtxs[i].cell;
            }
        }
    }
    ~AtlasTextureResource();

private:
    bool updateBuffer();

private:
    eAtlasSize m_size = eAtlasSize::kSize_1024x1024;

    std::unique_ptr<unsigned char[]> m_buffer;
public:
    std::vector<sAtlasCellCtx> m_cellCtxs;
};


typedef tResourceRef<AtlasTextureResource> AtlasTextureRef;

}