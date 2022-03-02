#include "AtlasTextureResource.h"
#include "tools/ioUtil.h"
#include <string>

using namespace Resource;

int AtlasTextureResource::tryPackTextures(const std::vector<TextureRef>& texs, bool useBigAtlas, int minmumTexNum)
{
    std::vector<eAtlasSize> trySizes = 
    { 
        eAtlasSize::kSize_256x256,  
        eAtlasSize::kSize_512x512,
        eAtlasSize::kSize_1024x1024,
        //eAtlasSize::kSize_2048x2048,
    };
    if (useBigAtlas)
        trySizes.push_back(eAtlasSize::kSize_2048x2048);
    
    static int atlasNum = 0;
    std::vector<sAtlasCell> outMeta;
    int packTexNum = texs.size();
    while (packTexNum >= minmumTexNum)
    {
        std::vector<sAtlasCell> meta;
        meta.resize(packTexNum);
        unsigned int validArea = 0;
        for (unsigned int i = 0; i < packTexNum; i++)
        {
            meta[i] = {"", "",i,0,0,(unsigned int)texs[i].get()->getWidth() + m_border * 2, (unsigned int)texs[i].get()->getHeight() + m_border * 2};
            validArea += meta[i].w * meta[i].h;
        }
        for (eAtlasSize size_: trySizes)
        {
            unsigned int size = static_cast<unsigned int>(size_);
            float usage = float(validArea) / float(size * size);
            bool canpack = usage <= 1 && usage >= 0.5f && Util::atlasCanPack(meta, size, outMeta);
            if (canpack && meta.size() >= minmumTexNum)
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
                m_metas = outMeta;

                std::string curIdx = std::to_string(atlasNum++);
                std::string size__ = std::to_string(size);
                std::string outputPath = std::string(_RESOURCE_PATH_) + "/atlasTexture/atlasUnpack" + curIdx + "(" + size__ + ").png";
                // 写入图片
                Util::writeTextureToPng(outputPath, size, size, data);
                std::cout << "[" << "atlasUnpack" + curIdx + "(" + size__ + ")" << "] atlas size: " << size << "\tcell num: " << meta.size() << "\tarea usage: " << usage << std::endl;
                return packTexNum;
            }
        }
        packTexNum /= 2;
    }

    return 0;
}