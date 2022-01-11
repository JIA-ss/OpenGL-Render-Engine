#include "resourceManager.h"
#include "tools/fileWatcher.h"
#include "tools/ioUtil.h"
#include <filesystem>
using namespace Resource;

IMPLEMENT_SINGLETON(ResourceManager)

std::string ResourceManager::getResourceRootPath(eResourceType type)
{
    switch (type)
    {
    case fragShader:
        return m_rootResourcePath + "\\shaders";
    case vertextShader:
        return m_rootResourcePath + "\\shaders";
    case jpgTexture:
        return m_rootResourcePath + "\\textures";
    case pngTexture:
        return m_rootResourcePath + "\\textures";
    default:
        assert(false);
    }
}

void ResourceManager::InitResource(eResourceType type)
{
    std::filesystem::path path = getResourceRootPath(type);
    for (auto it : std::filesystem::directory_iterator(path))
    {
        if (it.exists())
        {
            auto path = it.path();
            m_resourceMap[type][path.filename().string()] = Util::readFile(path.string().c_str());
            std::cout << "ResourceManager::InitResource " << path << path.filename() << std::endl;
        }
    }

    Util::FileWatcher* watcher = Util::FileWatcherManager::Instance()->CreateFileWatcher(path.string().c_str(), false);
    watcher->AddCreateFileCallback([this, type](const std::string& path, const std::string& name)
    {
        m_resourceMap[type][name] = Util::readFile((path + "\\" + name).c_str());
        std::cout << "Resource Create:" << name << std::endl;
    });

    watcher->AddDeleteFileCallback([this, type](const std::string& path, const std::string& name)
    {
        auto it = m_resourceMap[type].find(name);
        if (it != m_resourceMap[type].end())
        {
            m_resourceMap[type].erase(it);
        }
        std::cout << "Resource Delete:" << name << std::endl;
    });

    watcher->AddModifyFileCallback([this, type](const std::string& path, const std::string& name)
    {
        m_resourceMap[type][name] = Util::readFile((path + "\\" + name).c_str());
        std::cout << "Resource Modify:" << name << std::endl;
    });
}


void ResourceManager::Init()
{
    m_rootResourcePath = DEFAULT_RESOURCE_PATH;
    for (int i = ResourceTypeBegin + 1; i < ResourceTypeEnd; i++)
    {
        InitResource((eResourceType)i);
    }
}

const char* ResourceManager::GetResource(const char* name, eResourceType type)
{
    return m_resourceMap[type][name].c_str();
}

ResourceManager::~ResourceManager()
{
    std::string ShaderPath = getResourceRootPath(vertextShader);
    std::string TexturePath = getResourceRootPath(pngTexture);
    Util::FileWatcherManager::Instance()->DeleteFileWatcher(ShaderPath.c_str());
    Util::FileWatcherManager::Instance()->DeleteFileWatcher(TexturePath.c_str());
}