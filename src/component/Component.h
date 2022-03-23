#pragma once

#include "common/def.h"
#include <string>
#include <map>
#include <vector>

COMPONENT_NAMESPACE_BEGIN

class sEntity;
class sComponent
{
    friend class sEntity;
protected:
    // <componentId, <propertyId, propertyName>>
    static std::map<size_t, std::map<size_t, std::string>> ComponentDirevedClasses;
    static const size_t ComponentId;
    
    // <propertyId, propertyAddress>
    std::map<size_t, void*> m_properties;

    sEntity* m_entity;
protected:
    static size_t registerComponentId(const std::string& name);

    template<typename T>
    T registerPropertyId(const std::string& name, const T& val, void* prop);

public:
    static size_t GetStaticComponentId();

    template<typename T>
    T* get_property(const std::string& prop_name);

    std::vector<std::pair<std::string, void*>> get_properties();

    virtual size_t get_componentId() const;

    bool isType(const std::string& name);

    template<typename T>
    bool isType();

    sEntity* get_entity() const { return m_entity; };

    void bind_entity(sEntity* entity) { assert(!m_entity); m_entity = entity; }
};


template<typename T>
T* sComponent::get_property(const std::string& prop_name)
{
    size_t prop_id = std::hash<std::string>()(prop_name);
    auto it = m_properties.find(prop_id);
    if (it == m_properties.end())
        return nullptr;
    return static_cast<T*>(it->second);
}

template<typename T>
bool sComponent::isType()
{
    return get_componentId() == T::GetStaticComponentId();
}

template<typename T>
T sComponent::registerPropertyId(const std::string& name, const T& val, void* prop)
{
    size_t componentId = get_componentId();
    size_t propId = std::hash<std::string>()(name);
    m_properties[propId] = prop;
    ComponentDirevedClasses[componentId][propId] = name;
    return val;
}

COMPONENT_NAMESPACE_END