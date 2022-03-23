#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>


COMPONENT_NAMESPACE_BEGIN

class sTransform: public sComponent
{
    COMPONENT_DECLARE
    COMPONENT_PROPERTY(glm::vec3, position, glm::vec3(0.0f))
    COMPONENT_PROPERTY(glm::vec3, size, glm::vec3(1.0f))
    COMPONENT_PROPERTY(glm::vec3, rotation, glm::vec3(0.0f))
    COMPONENT_PROPERTY(glm::mat4, modelMatrice, glm::mat4(1.0f))
    COMPONENT_PROPERTY_WITHOUT_SETTER_AND_GETTER(sTransform*, parent, nullptr)
    COMPONENT_PROPERTY_WITHOUT_SETTER_AND_GETTER(std::set<sTransform*>, children, {})

public:
    void notifyPropertyChanged(const prop_variant& variant) override;

    bool deleteChild(sTransform* child);
    void addChild(sTransform* child);
    void setParent(sTransform* parent);
    sTransform* getParent();
    std::set<sTransform*> getChildren() const;
    int getChildrenSize() const;

    void OnDestroy() override;
    sComponent* Clone() override;

    bool IsMatriceDirty() const { return m_modelMatriceDirty; }
    void UpdateModelMatriceRecursively();
private:
    bool m_modelMatriceDirty = true;
};

COMPONENT_NAMESPACE_END