#include "AtlasTextureResource.h"
#include "tools/ioUtil.h"


using namespace Resource;

sAtlasCell AtlasTextureResource::addTexture(std::string_view name, TextureRef texture)
{
    unsigned int width = texture->getWidth(), height = texture->getHeight();
    unsigned int index = m_cellCtxs.size() + 1;
    m_cellCtxs.push_back(sAtlasCellCtx{ { name.data(), texture->getName(), index, 0, 0, width, height }, texture, false });
    return {};
}

void AtlasTextureResource::setSize(eAtlasSize size)
{
    m_size = size;
    auto bufferSize = getRawSize() * getRawSize() * 4;
    m_buffer.reset(new unsigned char[bufferSize]);
    memset(m_buffer.get(), 0, bufferSize);
}

bool AtlasTextureResource::pack()
{
    bool result = Util::atlasPack(*this);
    return updateBuffer() && result > 0;
}

bool AtlasTextureResource::updateBuffer()
{
    int SizeOfRGBA = 4;
    unsigned int rawSize = getRawSize();
    memset(m_buffer.get(), 0, rawSize * rawSize * 4);
    unsigned int size = rawSize;

    for (const auto& ctx : m_cellCtxs)
    {
        if (!ctx.packed) continue;

        int x = ctx.cell.x, y = ctx.cell.y, w = ctx.cell.w, h = ctx.cell.h;

        auto textureRowSize = w * SizeOfRGBA;
        auto atlasRowSize = size * SizeOfRGBA;

        TextureRef texture = ctx.texture;
        const unsigned char* textureOriginalBuffer = nullptr;
        int originBufferSize = 0;
        if (texture->getTextureData())
        {
            textureOriginalBuffer = texture->getTextureData();
            originBufferSize = texture->getSize();
        }
        
        int channels;
        w = texture->getWidth();
        h = texture->getHeight();


        for (auto row = 0; row < h; ++row)
            memcpy(m_buffer.get() + (row + y) * atlasRowSize + x * SizeOfRGBA, textureOriginalBuffer + row * textureRowSize, textureRowSize);

    }

    return true;
}