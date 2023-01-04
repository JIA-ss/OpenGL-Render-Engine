#pragma once

#include "Component.h"
#include "common/def.h"
#include "graphic/Texture.h"
#include "graphic/Material.h"
COMPONENT_NAMESPACE_BEGIN

class sSpriteRender: public sComponent
{
    COMPONENT_DECLARE
public:
    COMPONENT_PROPERTY(unsigned int, renderIdx, 1)
    COMPONENT_PROPERTY(std::vector<Graphic::Material*>, sharedMaterials, {})
};

COMPONENT_NAMESPACE_END