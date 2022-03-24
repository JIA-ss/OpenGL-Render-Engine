#include "Entity.h"
#include "component/Component.h"
#include "component/Transform.h"
#include "component/MeshRender.h"

#include "system/EntitySystem.h"
ENTITY_NAMESPACE_USE

std::map<size_t, entity_meta> sEntity::EntityDirevedClasses = std::map<size_t, entity_meta>();
ENTITY_IMPLEMENT(sEntity)

sEntity::sEntity()
{

}

sEntity::~sEntity()
{
    for (auto&&[compId, comp] : m_components)
    {
        delete comp;
    }
    m_components.clear();
}

void sEntity::Register()
{
    EntitySystem::Get()->registerEntity(this);
}

void sEntity::Unregister()
{
    EntitySystem::Get()->unregisterEntity(this);
}

bool sEntity::DestroyComponent(Component::sComponent* comp)
{
    if (!comp)
        return false;
    size_t compId = comp->get_componentId();
    auto it = m_components.find(compId);
    if (it == m_components.end())
        return false;
    m_components.erase(it);
    delete comp;
    return true;
}


void sEntity::Destroy(sEntity* entity)
{
    if (!entity)
        return;
    entity->Unregister();
    auto it = entity->m_components.begin();
    while (it != entity->m_components.end())
    {
        it->second->SetActive(false);
        it->second->OnDestroy();
        Component::sComponent* comp = it->second;
        it->second = nullptr;
        it = entity->m_components.erase(it);
        delete comp;
    }
    entity->m_components.clear();
    delete entity;
}



sEntity* sEntity::Clone(sEntity* entity)
{
    if (!entity)
        return nullptr;
    sEntity* new_entity = entity->Clone();

    sTransform* transform = entity->GetComponent<sTransform>();
    if (transform && transform->getChildrenSize() > 0)
    {
        sTransform* new_transform = new_entity->GetComponent<sTransform>();
        
        auto& children = transform->getChildren();
        for (auto& child : children)
        {
            sEntity* new_childEntity = Clone(child->get_entity());
            sTransform* new_child = new_childEntity->GetComponent<sTransform>();
            new_child->setParent(new_transform);
        }
        
        new_transform->setParent(transform->getParent());
    }
    
    new_entity->Register();
    return new_entity;
}

sEntity* sEntity::Clone()
{
    size_t entityId = get_entityId();
    entity_meta entityMeta = EntityDirevedClasses[entityId];
    void* newEntity = new char[entityMeta.size];
    memcpy(newEntity, this, entityMeta.size);

    sEntity* entity = static_cast<sEntity*>(newEntity);

    for (auto&&[compId, comp] : m_components)
    {
        Component::sComponent* new_comp = comp->Clone();
        new_comp->bind_entity(entity);
        new_comp->OnAwake();
        new_comp->IsActive() ? new_comp->OnEnable() : new_comp->OnDisable();
        entity->m_components[compId] = new_comp;
    }
    return entity;
}

void sEntity::SetActive(bool v)
{
    if (v != m_enable)
    {
        m_enable = v;
        if (v && IsHierarchyActive())
        {
            for (auto& [compId, comp] : m_components)
            {
                comp->OnEnable();
            }
        }
        else
        {
            for (auto& [compId, comp] : m_components)
            {
                comp->OnDisable();
            }
        }
    }
}

bool sEntity::IsHierarchyActive()
{
    if (!IsActiveSelf())
        return false;
    sTransform* trans = GetComponent<sTransform>();
    if (trans && trans->getParent())
    {
        return trans->getParent()->get_entity()->IsHierarchyActive();
    }
    return true;
}

bool sEntity::IsActiveSelf()
{
    return m_enable;
}