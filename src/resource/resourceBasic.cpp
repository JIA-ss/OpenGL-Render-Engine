#include "resourceBasic.h"
#include "tools/ioUtil.h"
#include <string.h>
#include "stb_image.h"
using namespace Resource;

ByteBuffer::ByteBuffer(const ByteBuffer &other)
{
    if (other.m_capacity > 0)
        m_buffer = new ubyte[m_capacity]();
    else
        m_buffer = nullptr;
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_capacityIncrement = other.m_capacityIncrement;
    if (m_size > 0)
        memcpy(m_buffer, other.m_buffer, m_size);
}

ByteBuffer::ByteBuffer(ByteBuffer &&other) noexcept
{
    m_buffer = other.m_buffer;
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_capacityIncrement = other.m_capacityIncrement;

    other.m_buffer = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

ByteBuffer &ByteBuffer::operator=(ByteBuffer &&other) noexcept
{
    clear();

    m_buffer = other.m_buffer;
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_capacityIncrement = other.m_capacityIncrement;

    other.m_buffer = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;

    return *this;
}

ByteBuffer &ByteBuffer::operator=(const ByteBuffer &other)
{
    if (m_capacity < other.m_size)
    {
        delete[] m_buffer;
        m_buffer = new ubyte[other.m_capacity]();
        m_capacity = other.m_capacity;
    }

    m_capacityIncrement = other.m_capacityIncrement;
    m_size = other.m_size;
    if (m_size > 0)
        memcpy(m_buffer, other.m_buffer, other.m_size);

    return *this;
}

void ByteBuffer::clear()
{
    if (m_buffer)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
}

void ByteBuffer::reverse(size_t capacity)
{
    if (capacity > m_capacity)
    {
        m_capacity = (capacity / m_capacityIncrement + 1) * m_capacityIncrement;
        ubyte *buf = new ubyte[m_capacity]();
        memcpy(buf, m_buffer, m_size);
        delete[] m_buffer;
        m_buffer = buf;
    }
}

void ByteBuffer::append(const void *src, size_t size)
{
    size_t newSize = m_size + size;
    if (newSize > m_capacity)
    {
        m_capacity = (newSize / m_capacityIncrement + 1) * m_capacityIncrement;
        ubyte *newBuffer = new ubyte[m_capacity]();
        memcpy(newBuffer, m_buffer, m_size);
        delete[] m_buffer;
        m_buffer = newBuffer;
    }

    if (size > 0 && src)
    {
        memcpy(m_buffer + m_size, src, size);
    }
    m_size = newSize;
}

sResourceRef sResourceRef::invalid;
sResourceRef::sResourceRef(const sResourceRef &other)
{
    m_resource = other.m_resource;
}
sResourceRef &sResourceRef::operator=(const sResourceRef &other)
{
    if (this != &other)
    {
        m_resource = other.m_resource;
    }
    return *this;
}

void ShaderResource::loadFromPath(const char *path)
{
    m_rawData.clear();

    std::filesystem::path path_(path);
    m_name = path_.filename().string();
    m_path = path;
    char *fileContent = nullptr;
    size_t size;
    fileContent = Util::readFile_Native(path, size, true);

    if (size > 0 && fileContent)
        m_rawData.append(fileContent, size);

    delete[] fileContent;
    m_isReady = true;
}

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

    unsigned char *idata = stbi_load_from_memory(fileContent, size, &m_width, &m_height, &m_channels, 0);
    delete[] fileContent;
    int imgSize = m_width * m_height * m_channels;
    m_rawData.append(idata, imgSize);

    stbi_image_free(idata);
    m_isReady = true;
}

TextureResource::~TextureResource()
{
}

template <class T>
tResourceRef<T> ResourceFactory::ImportResource(const char *path)
{
    std::shared_ptr<T> res = std::make_shared<T>();
    res->loadFromPath(path);
    tResourceRef<T> resRef(std::dynamic_pointer_cast<BaseResource>(res));
    return resRef;
}

template ShaderRef ResourceFactory::ImportResource<ShaderResource>(const char *path);
template TextureRef ResourceFactory::ImportResource<TextureResource>(const char *path);

sResourceRef ResourceFactory::ImportResourceByType(const char *path, eResourceType type)
{
    switch (type)
    {
    case shader:
        return ImportResource<ShaderResource>(path);
    case texture:
        return ImportResource<TextureResource>(path);
    default:
        return sResourceRef::invalid;
    }
}