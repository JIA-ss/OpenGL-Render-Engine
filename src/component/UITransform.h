#pragma once

#include "Component.h"
#include "common/def.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>


COMPONENT_NAMESPACE_BEGIN

class sUITransform: public sComponent
{
    COMPONENT_DECLARE
    COMPONENT_PROPERTY(glm::vec2, position, glm::vec2(0,0))
    COMPONENT_PROPERTY(glm::vec2, size, glm::vec2(200,200))
    COMPONENT_PROPERTY_WITHOUT_SETTER_AND_GETTER(sUITransform*, parent, nullptr);
    COMPONENT_PROPERTY_WITHOUT_SETTER_AND_GETTER(std::set<sUITransform*>, children, {})
public:
    void notifyPropertyChanged(const prop_variant& variant) override;
    void setParent(sUITransform* parent);
    void addChild(sUITransform* child);
    void deleteChild(sUITransform* child);
    bool IsDirty() { return m_dirty; }
    void UpdateModelMatriceRecursively();
private:
    bool m_dirty = false;
    glm::vec2 m_globalPosition;
};

COMPONENT_NAMESPACE_END