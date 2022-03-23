#pragma once

#include "Entity.h"

ENTITY_NAMESPACE_BEGIN

class sGameObject: public sEntity
{
    ENTITY_DECLARE
public:
    sGameObject() = default;
    sGameObject(const std::string& name) : sEntity(name) { }
    void CreateComponents() override;
};

ENTITY_NAMESPACE_END