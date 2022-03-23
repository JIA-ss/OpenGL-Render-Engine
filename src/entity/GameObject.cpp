#include "GameObject.h"
#include "component/Transform.h"

COMPONENT_NAMESPACE_USING
ENTITY_NAMESPACE_USE

ENTITY_IMPLEMENT(sGameObject)

void sGameObject::CreateComponents()
{
    AddComponent<sTransform>();
}