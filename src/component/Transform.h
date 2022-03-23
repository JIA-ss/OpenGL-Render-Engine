#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

COMPONENT_NAMESPACE_BEGIN

class sTransform: public sComponent
{
    COMPONENT_DECLARE
    COMPONENT_PROPERTY(glm::vec3, position, glm::vec3(1.0f))
    COMPONENT_PROPERTY(sTransform*, parent, nullptr);
    COMPONENT_PROPERTY(std::vector<sTransform*>, children, {});
};

COMPONENT_NAMESPACE_END