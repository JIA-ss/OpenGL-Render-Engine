#pragma once

#include "common/def.h"
#include <string>
#include <map>
#include <vector>
#include <typeinfo>
namespace Entity { class sEntity; }

COMPONENT_NAMESPACE_BEGIN

class sComponent
{
public:
    struct prop_meta
    {
        std::string name;
        size_t size;
        prop_meta() = default;
    };

    struct prop_variant
    {
        sComponent::prop_meta meta;
        void* ptr;
        prop_variant() = default;
    };
    struct comp_meta
    {
        std::string name;
        size_t size;
        std::map<size_t, sComponent::prop_meta> props = { };
        comp_meta() = default;
    };

protected:
    static const size_t ComponentId;
    
    // <propertyId, propertyAddress>
    std::map<size_t, prop_variant> m_properties;

    // <componentId, <propertyId, propertyName>>
    static std::map<size_t, comp_meta> ComponentDirevedClasses;

    Entity::sEntity* m_entity;
protected:
    template<typename T>
    static size_t registerComponentId(const std::string& name);

    template<typename T>
    T registerPropertyId(const std::string& name, const T& val, void* prop);

    friend class Entity::sEntity;
    virtual sComponent* Clone();
public:
    static size_t GetStaticComponentId();

    template<typename T>
    T* get_property(const std::string& prop_name);

    void* get_property(size_t propId);

    std::map<size_t, prop_variant> get_properties();

    virtual size_t get_componentId() const;

    bool isType(const std::string& name);

    template<typename T>
    bool isType();

    Entity::sEntity* get_entity() const { return m_entity; };

    void bind_entity(Entity::sEntity* entity) { assert(!m_entity); m_entity = entity; }
};


template<typename T>
T* sComponent::get_property(const std::string& prop_name)
{
    size_t prop_id = std::hash<std::string>()(prop_name);
    auto it = m_properties.find(prop_id);
    if (it == m_properties.end())
        return nullptr;
    return static_cast<T*>(it->second.ptr);
}

template<typename T>
bool sComponent::isType()
{
    return get_componentId() == T::GetStaticComponentId();
}

template<typename T>
size_t sComponent::registerComponentId(const std::string& name)
{
    size_t id = std::hash<std::string>()(name);
    auto it = ComponentDirevedClasses.find(id);
    if (it != ComponentDirevedClasses.end())
        return id;
    
    sComponent::comp_meta meta{name, sizeof(T)};
    ComponentDirevedClasses[id] = meta;
    return id;
}



template<typename T>
T sComponent::registerPropertyId(const std::string& name, const T& val, void* prop)
{
    size_t componentId = get_componentId();
    size_t propId = std::hash<std::string>()(name);
    auto it = m_properties.find(propId);
    if (it != m_properties.end())
        return val;
    prop_meta meta { name, sizeof(T) };
    prop_variant variant { meta, prop };
    
    m_properties[propId] = variant;
    ComponentDirevedClasses[componentId].props[propId] = meta;
    return val;
}

COMPONENT_NAMESPACE_END