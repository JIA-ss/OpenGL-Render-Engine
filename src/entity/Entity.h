#pragma once
#include "component/Component.h"
#include "common/def.h"

#include <vector>
#include <unordered_map>
ENTITY_NAMESPACE_BEGIN


class sEntity
{
public:
    sEntity();
    ~sEntity();
    virtual sEntity* Clone();
    virtual void CreateComponents();

    template<typename T, typename...Args>
    T* AddComponent(Args &&... params);

    template<typename T>
    T* GetComponent();

    template<typename T>
    bool DeleteComponent();
protected:
    std::unordered_map<size_t ,Component::sComponent*> m_components;
};




template<typename T, typename...Args>
T* sEntity::AddComponent(Args &&... params)
{
    size_t compId = T::GetStaticComponentId();
    auto it = m_components.find(compId);
    if (it != m_components.end())
        return nullptr;
    
    T* comp = new T(params);
    comp->bind_entity(this);
    m_components[compId] = comp;
    return comp;
}

template<typename T>
T* sEntity::GetComponent()
{
    size_t compId = T::GetStaticComponentId();
    auto it = m_components.find(compId);
    if (it != m_components.end())
        return nullptr;
    return it->second;
}

template<typename T>
bool sEntity::DeleteComponent()
{
    size_t compId = T::GetStaticComponentId();
    auto it = m_components.find(compId);
    if (it != m_components.end())
        return false;
    m_components.erase(it);
    return true;
}

ENTITY_NAMESPACE_END
