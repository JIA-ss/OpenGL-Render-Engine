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
    unsigned int packed = 0;
};


class AtlasTextureResource : private TextureResource
{
private:
    std::vector<sAtlasCell> m_metas;
    unsigned int m_border = 5;
public:
    int tryPackTextures(const std::vector<TextureRef>& texs, bool useBigAtlas = false, int minmumTexNum = 2);
    int getCellNums() { return m_metas.size(); }
    int getSize() { return m_width; }
};

typedef tResourceRef<AtlasTextureResource> AtlasTextureRef;

}