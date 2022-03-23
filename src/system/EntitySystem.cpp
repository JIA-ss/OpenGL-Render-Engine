#include "EntitySystem.h"

void EntitySystem::registerEntity(Entity::sEntity* entity)
{
    m_entities.insert(entity);
}

void EntitySystem::unregisterEntity(Entity::sEntity* entity)
{
    m_entities.erase(entity);
}