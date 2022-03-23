#pragma once

#include "common/def.h"
#include "component/Component.h"

#include <vector>
#include <unordered_map>


ENTITY_NAMESPACE_BEGIN

class sEntity
{
public:
    sEntity() { };
    ~sEntity();

    virtual void CreateComponents() { };

    template<typename T, typename...Args>
    T* AddComponent(Args &&... params);

    template<typename T>
    T* GetComponent();

    template<typename T>
    bool DestroyComponent();

    bool DestroyComponent(Component::sComponent* comp);
protected:
    void Register();
    void Unregister();
    virtual sEntity* Clone();

public:
    template<typename T, typename...Args>
    static T* Create(Args&&... params);

    static void Destroy(sEntity*);

    static sEntity* Clone(sEntity* entity);

protected:
    std::unordered_map<size_t ,Component::sComponent*> m_components;
};


template<typename T, typename...Args>
T* sEntity::Create(Args&&...params)
{
    T* entity = new T(params...);
    entity->Register();
    return entity;
}


template<typename T, typename...Args>
T* sEntity::AddComponent(Args &&... params)
{
    size_t compId = T::GetStaticComponentId();
    auto it = m_components.find(compId);
    if (it != m_components.end())
        return nullptr;
    
    T* comp = new T(params...);
    comp->bind_entity(this);
    m_components[compId] = comp;
    return comp;
}

template<typename T>
T* sEntity::GetComponent()
{
    size_t compId = T::GetStaticComponentId();
    auto it = m_components.find(compId);
    if (it == m_components.end())
        return nullptr;
    return static_cast<T*>(it->second);
}

template<typename T>
bool sEntity::DestroyComponent()
{
    size_t compId = T::GetStaticComponentId();
    auto it = m_components.find(compId);
    if (it == m_components.end())
        return false;
    Component::sComponent* comp = it->second;
    m_components.erase(it);
    delete comp;
    return true;
}

ENTITY_NAMESPACE_END
