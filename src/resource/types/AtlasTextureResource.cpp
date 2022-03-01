#include "AtlasTextureResource.h"
#include "tools/ioUtil.h"
#include <string>

using namespace Resource;

int AtlasTextureResource::tryPackTextures(const std::vector<TextureRef>& texs)
{
    static std::vector<eAtlasSize> trySizes = 
    { 
        Resource::eAtlasSize::kSize_256x256,  
        Resource::eAtlasSize::kSize_512x512,
        Resource::eAtlasSize::kSize_1024x1024,
        //Resource::eAtlasSize::kSize_2048x2048,
    };
    static int atlasNum = 0;
    std::vector<sAtlasCell> outMeta;
    int packTexNum = texs.size();
    while (packTexNum > 0)
    {
        std::vector<sAtlasCell> meta;
        meta.resize(packTexNum);
        for (unsigned int i = 0; i < packTexNum; i++)
        {
            meta[i] = {"", "",i,0,0,(unsigned int)texs[i].get()->getWidth() + m_border * 2, (unsigned int)texs[i].get()->getHeight() + m_border * 2};
        }
        for (eAtlasSize size_: trySizes)
        {
            unsigned int size = static_cast<unsigned int>(size_);
            bool canpack =  Util::atlasCanPack(meta, size, outMeta);
            if (canpack)
            {
                std::vector<const unsigned char*> texData;
                for (int i = 0; i < packTexNum; i++)
                {
                    unsigned char* borderData = Util::addBorderForTexture(texs[i].get()->getTextureData(), texs[i].get()->getWidth(), texs[i].get()->getHeight(), m_border);
                    texData.push_back(borderData);
                }
                unsigned char* data = Util::packAtlasWithMeta(outMeta, size, texData);
                m_rawData.append(data, size * size * 4);
                m_width = size;
                m_height = size;
                m_channels = 4;
                m_metas = meta;

                std::string curIdx = std::to_string(atlasNum++);
                std::string size__ = std::to_string(size);
                std::string outputPath = std::string(_RESOURCE_PATH_) + "/atlasTexture/atlasUnpack" + curIdx + "(" + size__ + ").png";
                // 写入图片
                Util::writeTextureToPng(outputPath, size, size, data);

                return packTexNum;
            }
        }
        packTexNum /= 2;
    }

    return 0;
}