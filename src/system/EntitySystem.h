#pragma once
#include "System.h"
#include "entity/Entity.h"
#include <set>

class EntitySystem : public System
{
    SYSTEM_STATIC_TYPE(EntitySystem, System::Type::EntitySystem)
public:
    EntitySystem() : System(System::Type::EntitySystem) { }

public:
    void registerEntity(Entity::sEntity* entity);
    void unregisterEntity(Entity::sEntity* entity);

private:
    std::set<Entity::sEntity*> m_entities;
};