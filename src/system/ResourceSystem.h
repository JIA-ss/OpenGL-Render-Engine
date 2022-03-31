#pragma once
#include "System.h"
#include "resource/resourceBasic.h"
#include "graphic/GraphicBase.h"

#include <filesystem>
#include <unordered_map>
using namespace Resource;
using namespace Graphic;

class ResourceSystem;
class GraphicBaseResource
{
    friend class ResourceSystem;
protected:
    std::string name;
    void* graphic;
public:
    GraphicBaseResource() = default;
    bool operator == (const GraphicBaseResource& s) const { return graphic == s.graphic; }
};

template<typename T>
class GraphicResource : public GraphicBaseResource
{
public:
    GraphicResource() = default;
    GraphicResource(const std::string& _name, GraphicBase* _graphic) : name(_name), graphic(_graphic) { }
    T* GetGraphic() { return static_cast<T*>(graphic); }
    std::string& GetName() { return name; }
};



class ResourceSystem : public System
{
    SYSTEM_STATIC_TYPE(ResourceSystem, System::Type::ResourceSystem)
public:
    ResourceSystem() : System(System::Type::ResourceSystem) { }

    void Init() override;
    void Update() override;
    void UnInit() override;
public:
    sResourceRef ImportResource(const char* path, eResourceType type);
    sResourceRef GetResource(const char* name, eResourceType type, bool loadIfNull = true);
    void DeleteResource(const char* name, eResourceType type);

public:
    template <typename T, typename ... Args>
    static GraphicResource<T> LoadGraphicResource(const std::string& name, Args&&... args);

    template <typename T>
    static T* GetGraphic(const std::string& name);

private:
    std::string getResourceRootPath(eResourceType type);
    void InitResource(eResourceType type);
    void InitShaderResource(const std::string& path = "",const std::string& relative_path = "");
    void InitTextureResource(const std::string& path = "",const std::string& relative_path = "");
    void InitAtlasTextureResource(const std::string& path = "",const std::string& relative_path = "");
private:
    std::filesystem::path m_rootResourcePath;
    std::unordered_map<eResourceType, std::unordered_map<std::string, sResourceRef>> m_resourceMap;
    std::unordered_map<size_t, std::unordered_map<std::string, GraphicBase*>> m_graphices;
};



template <typename T, typename ... Args>
GraphicResource<T> ResourceSystem::LoadGraphicResource(const std::string& name, Args&&... args)
{
    size_t graphicId = T::GetStaticGraphicId();
    auto& graphicMap = ResourceSystem::Get()->m_graphices[graphicId];
    GraphicResource<T> res;
    std::string _name_ = name;
    auto it = graphicMap.find(_name_);
    int i = 0;
    while (it != graphicMap.end())
    {
        _name_ = name + "[" + std::to_string(++i) + "]";
        it = graphicMap.find(_name_);
    }

    T* graphic = new T(args...);
    graphicMap[name] = graphic;
    res.name = name;
    res.graphic = graphic;
    return res;
}

template <typename T>
T* ResourceSystem::GetGraphic(const std::string& name)
{
    size_t graphicId = T::GetStaticGraphicId();
    auto& graphicMap = ResourceSystem::Get()->m_graphices[graphicId];
    auto it = graphicMap.find(name);
    if (it == graphicMap.end())
        return nullptr;
    return static_cast<T*>(it->second);
}