#pragma once
#include "component/Component.h"
#define ENTITY_NAMESPACE_BEGIN      \
    namespace Entity {                  

#define ENTITY_NAMESPACE_END        \
    }

#define ENTITY_NAMESPACE_USE        \
    using namespace Entity;

ENTITY_NAMESPACE_BEGIN


class Entity
{
public:
    virtual Entity* Clone() = 0;
};


ENTITY_NAMESPACE_END
