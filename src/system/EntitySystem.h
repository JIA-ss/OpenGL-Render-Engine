#pragma once
#include "System.h"
#include "entity/Entity.h"
#include "component/Transform.h"
#include <set>

COMPONENT_NAMESPACE_USING
ENTITY_NAMESPACE_USE

class EntitySystem : public System
{
    SYSTEM_STATIC_TYPE(EntitySystem, System::Type::EntitySystem)
public:
    EntitySystem() : System(System::Type::EntitySystem) { }

public:
    void registerEntity(Entity::sEntity* entity);
    void unregisterEntity(Entity::sEntity* entity);

public:
    void onParentChanged(sTransform* cur, sTransform* oldParent);
private:
    std::set<Entity::sEntity*> m_entities;
    std::set<Entity::sEntity*> m_roots;
};