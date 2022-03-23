#pragma once

#include "common/def.h"
#include "component/Component.h"

#include <vector>
#include <unordered_map>


ENTITY_NAMESPACE_BEGIN

class sEntity
{
public:
    sEntity();
    ~sEntity();

    virtual void CreateComponents() { };

    template<typename T, typename...Args>
    T* AddComponent(Args &&... params);

    template<typename T>
    T* GetComponent();

    template<typename T>
    bool DestroyComponent();

    bool DestroyComponent(Component::sComponent* comp);

    virtual size_t get_entityId() const { return EntityId; };
protected:
    void Register();
    void Unregister();
    virtual sEntity* Clone();

    static size_t GetStaticEntityId();
    
    template<typename T>
    static size_t registerEntityId(const std::string& name);
public:
    template<typename T, typename...Args>
    static T* Create(Args&&... params);

    static void Destroy(sEntity*);

    static sEntity* Clone(sEntity* entity);

    void SetActive(bool v);
    bool IsActiveSelf();
    bool IsHierarchyActive();
protected:
    static std::map<size_t, entity_meta> EntityDirevedClasses;
    static const size_t EntityId;
    std::unordered_map<size_t ,Component::sComponent*> m_components;
    bool m_enable = true;
};


template<typename T, typename...Args>
T* sEntity::Create(Args&&...params)
{
    T* entity = new T(params...);
    entity->CreateComponents();
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

    // dont need to rtti record comps, because components is dynamic added
    T* comp = new T(params...);
    comp->bind_entity(this);
    comp->OnAwake();
    comp->SetActive(true);
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
    comp->SetActive(false);
    comp->OnDestroy();
    delete comp;
    return true;
}


template<typename T>
size_t sEntity::registerEntityId(const std::string& name)
{
    size_t id = std::hash<std::string>()(name);
    auto it = EntityDirevedClasses.find(id);
    if (it != EntityDirevedClasses.end())
        return id;
    
    entity_meta meta{name, sizeof(T), {}};
    EntityDirevedClasses[id] = meta;
    return id;
}

ENTITY_NAMESPACE_END
