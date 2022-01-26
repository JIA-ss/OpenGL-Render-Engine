#include "resourceManager.h"
#include "tools/fileWatcher.h"
#include <filesystem>
using namespace Resource;

IMPLEMENT_SINGLETON(ResourceManager)

std::string ResourceManager::getResourceRootPath(eResourceType type)
{
    switch (type)
    {
    case shader:
        return m_rootResourcePath + "\\shaders";
    case texture:
        return m_rootResourcePath + "\\textures";
    default:
        assert(false);
    }
}

void ResourceManager::ImportResource(const char* path, eResourceType type)
{

}

void ResourceManager::InitShaderResource()
{
    std::filesystem::path path = getResourceRootPath(shader);
    for (auto it : std::filesystem::directory_iterator(path))
    {
        if (!it.exists())
            continue;
        std::filesystem::path it_path = it.path();
        std::string path_str = it_path.string();
        sResourceRef res = ResourceFactory::ImportResource<ShaderResource>(path_str.c_str());
        m_resourceMap[shader][it_path.filename().string()] = res;
        std::cout << "ResourceManager::InitResource " << path << path.filename() << std::endl;
    }
}

void ResourceManager::InitTextureResource()
{
    std::filesystem::path path = getResourceRootPath(shader);
    for (auto it : std::filesystem::directory_iterator(path))
    {
        if (!it.exists())
            continue;
        std::filesystem::path it_path = it.path();
        std::string path_str = it_path.string();
        sResourceRef res = ResourceFactory::ImportResource<TextureResource>(path_str.c_str());
        m_resourceMap[shader][it_path.filename().string()] = res;
        std::cout << "ResourceManager::InitResource " << path << path.filename() << std::endl;
    }
}

void ResourceManager::InitResource(eResourceType type)
{
    std::filesystem::path path = getResourceRootPath(type);

    switch (type)
    {
    case shader:
        InitShaderResource();
        break;
    case texture:
        break;
        InitTextureResource();
    default:
        break;
    }


    Util::FileWatcher* watcher = Util::FileWatcherManager::Instance()->GetFileWatcher(path.string().c_str());
    if (watcher != nullptr)
        return;
    watcher = Util::FileWatcherManager::Instance()->CreateFileWatcher(path.string().c_str(), false);
    watcher->AddCreateFileCallback([this, type](const std::string& path, const std::string& name)
    {
        ImportResource(path.c_str(), type);
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
        ImportResource(path.c_str(), type);
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

sResourceRef ResourceManager::GetResource(const char* name, eResourceType type)
{
    if (m_resourceMap[type].find(name) == m_resourceMap[type].end())
        return sResourceRef::invalid;
    return m_resourceMap[type][name];
}

void ResourceManager::DeleteResource(const char* name, eResourceType type)
{
    m_resourceMap[type].erase(name);
}

ResourceManager::~ResourceManager()
{
    std::string ShaderPath = getResourceRootPath(shader);
    std::string TexturePath = getResourceRootPath(texture);
    Util::FileWatcherManager::Instance()->DeleteFileWatcher(ShaderPath.c_str());
    Util::FileWatcherManager::Instance()->DeleteFileWatcher(TexturePath.c_str());

    m_resourceMap.clear();
}