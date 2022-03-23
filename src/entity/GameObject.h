#pragma once

#include "Entity.h"

ENTITY_NAMESPACE_BEGIN

class sGameObject: public sEntity
{
    ENTITY_DECLARE
    void CreateComponents() override;
};

ENTITY_NAMESPACE_END