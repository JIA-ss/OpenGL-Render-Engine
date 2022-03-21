#pragma once
#include "System.h"
#include "resource/resourceBasic.h"

#include <filesystem>
#include <unordered_map>
using namespace Resource;

class ResourceSystem : public System
{
    SYSTEM_STATIC_TYPE(ResourceSystem, System::Type::ResourceSystem)
public:
    ResourceSystem() : System(System::Type::ResourceSystem) { }

    void Init() override;
    void Update() override;
    void UnInit() override;
public:
    void ImportResource(const char* path, eResourceType type);
    sResourceRef GetResource(const char* name, eResourceType type);
    void DeleteResource(const char* name, eResourceType type);
private:
    std::string getResourceRootPath(eResourceType type);
    void InitResource(eResourceType type);
    void InitShaderResource(const std::string& path = "",const std::string& relative_path = "");
    void InitTextureResource(const std::string& path = "",const std::string& relative_path = "");
    void InitAtlasTextureResource(const std::string& path = "",const std::string& relative_path = "");
private:
    std::filesystem::path m_rootResourcePath;
    std::unordered_map<eResourceType, std::unordered_map<std::string, sResourceRef>> m_resourceMap;
};