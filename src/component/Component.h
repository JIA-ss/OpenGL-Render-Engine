#pragma once

#include "common/def.h"
#include "common/custom_rtti.h"

#include <string>
#include <map>
#include <vector>
#include <typeinfo>
namespace Entity { class sEntity; }

COMPONENT_NAMESPACE_BEGIN

class sComponent
{
protected:
    static const size_t ComponentId;
    
    // <propertyId, propertyAddress>
    std::map<size_t, prop_variant> m_properties;

    // <componentId, <propertyId, propertyName>>
    static std::map<size_t, comp_meta> ComponentDirevedClasses;

    Entity::sEntity* m_entity;

    bool m_enable = false;
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

    virtual void notifyPropertyChanged(const prop_variant& prop);

    bool isType(const std::string& name);

    template<typename T>
    bool isType();

    Entity::sEntity* get_entity() const { return m_entity; };

    void bind_entity(Entity::sEntity* entity) { assert(!m_entity); m_entity = entity; }

public:
    // life time behavior
    virtual void OnAwake() { }
    virtual void OnEnable() { }
    virtual void OnUpdate() { }
    virtual void OnDisable() { }
    virtual void OnDestroy() { }

    void SetActive(bool v);
    bool IsActive();
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
    
    comp_meta meta{name, sizeof(T)};
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