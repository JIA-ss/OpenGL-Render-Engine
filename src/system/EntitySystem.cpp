#include "EntitySystem.h"

void EntitySystem::registerEntity(Entity::sEntity* entity)
{
    m_entities.insert(entity);
}

void EntitySystem::unregisterEntity(Entity::sEntity* entity)
{
    m_entities.erase(entity);
    m_roots.erase(entity);
}

void EntitySystem::onParentChanged(sTransform* cur, sTransform* oldParent)
{
    if (!oldParent && cur->getParent())
    {
        m_roots.erase(cur->get_entity());
    }
    else if (!cur->getParent())
    {
        m_roots.insert(cur->get_entity());
    }
}