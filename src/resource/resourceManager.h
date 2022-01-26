#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "common/def.h"
#include "tools/fileWatcher.h"
#include "tools/ioUtil.h"
#include "resourceBasic.h"
#include <unordered_map>
namespace Resource
{



class ResourceManager
{
    DECLARE_SINGLETON(ResourceManager)
public:
    ~ResourceManager();
    void Init();
    void ImportResource(const char* path, eResourceType type);
    sResourceRef GetResource(const char* name, eResourceType type);
    void DeleteResource(const char* name, eResourceType type);
private:
    std::string getResourceRootPath(eResourceType type);
    void InitResource(eResourceType type);
    void InitShaderResource();
    void InitTextureResource();
private:
    std::string m_rootResourcePath;
    std::unordered_map<eResourceType, std::unordered_map<std::string, sResourceRef>> m_resourceMap;
};


}


#endif