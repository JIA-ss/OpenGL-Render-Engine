#include "Entity.h"
#include "component/Component.h"
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
    delete entity;
}



sEntity* sEntity::Clone(sEntity* entity)
{
    sEntity* new_entity = entity->Clone();
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
        entity->m_components[compId] = new_comp;
    }
    return entity;
}