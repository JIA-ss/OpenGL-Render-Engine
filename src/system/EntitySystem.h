#pragma once
#include "System.h"
#include "component/UITransform.h"
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
    void Update() override;
    void UnInit() override;
public:
    void registerEntity(Entity::sEntity* entity);
    void unregisterEntity(Entity::sEntity* entity);

public:
    void onParentChanged(sTransform* cur, sTransform* oldParent);
    void onTransformChanged(sTransform* transform);
    void onUITransformChanged(sUITransform* transform);
private:
    void refreshDirtyTransforms();
private:
    std::set<Entity::sEntity*> m_entities;
    std::set<Entity::sEntity*> m_roots;

    std::set<Component::sTransform*> m_dirtyTransforms;
    std::set<Component::sUITransform*> m_dirtyUITransforms;
};