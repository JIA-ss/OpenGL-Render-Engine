#include "resourceManager.h"
#include "tools/fileWatcher.h"
#include "resource/types/TextureResource.h"
#include "resource/types/ShaderResource.h"
#include <filesystem>
#include <algorithm>
using namespace Resource;

IMPLEMENT_SINGLETON(ResourceManager)

std::string ResourceManager::getResourceRootPath(eResourceType type)
{
    switch (type)
    {
    case shader:
        return (m_rootResourcePath / "shaders").string();
    case texture:
        return (m_rootResourcePath / "textures").string();
    case atlasTexture:
        return (m_rootResourcePath / "textures").string();
    default:
        assert(false);
    }
}

void ResourceManager::ImportResource(const char* path, eResourceType type)
{
    std::filesystem::path path_(path);
    std::string filename = path_.filename().string();
    sResourceRef ref = GetResource(filename.c_str(), type);
    if (ref.isNull())
    {
        ref = ResourceFactory::ImportResourceByType(path, type);
        m_resourceMap[type][filename] = ref;
    }
    else
    {
        ref->loadFromPath(path);
    }

    if (type == shader)
    {
        ShaderRef sRef = ref;
        std::cout << "===" << path << "=== " << "content before" << std::endl;
        std::cout << sRef->getContent() << std::endl;
        std::cout << "===" << path << "=== " << "content over" << std::endl;
    }
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
        //std::cout << "ResourceManager::InitResource " << path_str << it_path.filename() << std::endl;
    }
}

void ResourceManager::InitTextureResource()
{
    std::filesystem::path path = getResourceRootPath(texture);
    for (auto it : std::filesystem::directory_iterator(path))
    {
        if (!it.exists())
            continue;
        std::filesystem::path it_path = it.path();
        std::string path_str = it_path.string();
        sResourceRef res = ResourceFactory::ImportResource<TextureResource>(path_str.c_str());
        m_resourceMap[texture][it_path.filename().string()] = res;
        //std::cout << "ResourceManager::InitResource " << path_str << it_path.filename() << std::endl;
    }
}

void ResourceManager::InitAtlasTextureResource()
{
    return;
    
    auto start = std::chrono::system_clock::now();

    int texNums = m_resourceMap[texture].size();
    int curTex = 0;
    std::vector<TextureRef> texs;
    for (auto[filename, ref] : m_resourceMap[texture])
    {
        texs.push_back(ref);
    }

    sort(texs.begin(), texs.end(), [](const TextureRef& v1, const TextureRef& v2)
    {
        return v1.get()->getWidth() * v1.get()->getHeight() > v2.get()->getWidth() * v2.get()->getHeight();
    });

    std::vector<TextureRef> failedPackTexs;
    std::vector<TextureRef> finalFailedPackedTexs;


    std::shared_ptr<AtlasTextureResource> res = std::make_shared<AtlasTextureResource>();

    int packedTexNums = 0;
    
    int packNum = res->tryPackTextures(texs);
    curTex += packNum;

    packedTexNums += packNum;
    while( curTex < texNums)
    {
        res = std::make_shared<AtlasTextureResource>();
        packNum = res->tryPackTextures(std::vector<TextureRef>(texs.begin() + curTex, texs.end()), true, 10, 0.7f);
        if (packNum > 0)
        {
            m_resourceMap[atlasTexture][std::to_string(m_resourceMap[atlasTexture].size())] = std::dynamic_pointer_cast<BaseResource>(res);
            curTex += packNum;
            packedTexNums += packNum;
        }
        else
        {
            failedPackTexs.push_back(texs[curTex++]);
        }
    }

    curTex = 0;
    while (curTex < failedPackTexs.size())
    {
        res = std::make_shared<AtlasTextureResource>();
        packNum = res->tryPackTextures(std::vector<TextureRef>(failedPackTexs.begin() + curTex, failedPackTexs.end()), true);
        std::cout << "atlas size: " << res->getSize() << "\t cell num: " << res->getCellNums() << std::endl;
        if (packNum > 0)
        {
            m_resourceMap[atlasTexture][std::to_string(m_resourceMap[atlasTexture].size())] = std::dynamic_pointer_cast<BaseResource>(res);
            curTex += packNum;
            packedTexNums += packNum;
        }
        else
        {
            finalFailedPackedTexs.push_back(failedPackTexs[curTex++]);
        }
    }

    for (int i = 0; i < finalFailedPackedTexs.size(); i++)
    {
        std::cout << "Failed packed Texture: " ;
        std::cout << finalFailedPackedTexs[i]->getName();
        std::cout << " (" << finalFailedPackedTexs[i]->getWidth() << ", " << finalFailedPackedTexs[i]->getHeight() << ")";
        std::cout << std::endl;
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "total texture num: " << texNums << "\tpacked texture num: " << packedTexNums << std::endl;
    std::cout << "pack " << m_resourceMap[atlasTexture].size() << " atlas" << std::endl;
    std::cout << "total Pack Time: " << elapsed_seconds.count() * 1000 << " ms" << std::endl;
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
        InitTextureResource();
        break;
    case atlasTexture:
        InitAtlasTextureResource();
        break;
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
    //m_rootResourcePath = Util::getSrcPath().parent_path() / "resources";
    //m_rootResourcePath = std::filesystem::path("F:\\StudyProj\\openGLStudy\\review\\resources");
    m_rootResourcePath = Util::getResourcePath();
    if (!std::filesystem::exists(m_rootResourcePath))
    {
        std::cout << "Get Path of Root Resource Failed: " << m_rootResourcePath << std::endl;
        assert(false);
        return;
    }
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