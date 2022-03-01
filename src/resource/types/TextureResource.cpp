#include "TextureResource.h"
#include "tools/ioUtil.h"

using namespace Resource;

void TextureResource::loadFromPath(const char *path)
{
    m_rawData.clear();

    size_t size;
    ubyte *fileContent = (ubyte *)Util::readFile_Native(path, size, false);
    if (size < 4)
    {
        std::cout << "[TextureResource] load image file failed" << std::endl;
        if (fileContent)
            delete[] fileContent;
        return;
    }

    ubyte data[4] = {0};
    memcpy(data, fileContent, 4);

    bool validFileType = false;
    // jpg
    if (data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF)
    {
        validFileType = true;
    }
    // png
    else if (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47)
    {
        validFileType = true;
    }

    if (!validFileType)
    {
        std::cout << "[TextureResource] unsupported image file type" << std::endl;
        delete[] fileContent;
        return;
    }

    unsigned char *idata = Util::loadTextureFromMemory(fileContent, size, &m_width, &m_height, &m_channels, 4);
    delete[] fileContent;

    int imgSize = m_width * m_height * 4;
    m_rawData.append(idata, imgSize);

    Util::freeTextureBuffer(idata);
    m_isReady = true;
}

TextureResource::~TextureResource()
{
}
