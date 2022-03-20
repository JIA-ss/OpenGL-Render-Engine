#pragma once
#include "System.h"
#include "entity/Entity.h"
class EntitySystem : public System
{
    SYSTEM_STATIC_TYPE(EntitySystem, System::Type::EntitySystem)
public:
    EntitySystem() : System(System::Type::EntitySystem) { }
    //void Init() override;
    //void Update() override;

public:
    template<typename T>
    T* CreateEntity();

    void DestroyEntity(Entity::Entity* entity);
};