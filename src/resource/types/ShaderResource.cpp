#include "ShaderResource.h"
#include "tools/ioUtil.h"

using namespace Resource;

bool ShaderResource::loadFromPath(const char *path)
{
    m_rawData.clear();

    std::filesystem::path path_(path);
    m_name = path_.filename().string();
    m_path = path;
    char *fileContent = nullptr;
    size_t size;
    fileContent = Util::readFile_Native(path, size, true);
    
    if (!fileContent)
        return false;

    if (size > 0 && fileContent)
        m_rawData.append(fileContent, size);

    delete[] fileContent;
    m_isReady = true;

    return true;
}
