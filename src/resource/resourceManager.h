#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "common/def.h"
#include "tools/fileWatcher.h"
#include <unordered_map>
namespace Resource
{

enum eResourceType
{
    ResourceTypeBegin = 0,
    vertextShader = 1,
    fragShader = 2,
    pngTexture = 3,
    jpgTexture = 4,
    ResourceTypeEnd = 5
};

class ResourceManager
{
    DECLARE_SINGLETON(ResourceManager)
public:
    ~ResourceManager();
    void Init();
    const char* GetResource(const char* name, eResourceType type);
private:
    std::string getResourceRootPath(eResourceType type);
    void InitResource(eResourceType type);
private:
    std::string m_rootResourcePath;
    std::unordered_map<eResourceType, std::unordered_map<std::string, std::string>> m_resourceMap;
};


}


#endif