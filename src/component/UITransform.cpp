#include "UITransform.h"
#include "common/def.h"
#include "system/EntitySystem.h"

COMPONENT_NAMESPACE_USING

COMPONENT_IMPLEMENT(sUITransform)

void sUITransform::notifyPropertyChanged(const prop_variant& variant)
{
    m_dirty = true;
    EntitySystem::Get()->onUITransformChanged(this);
}

void sUITransform::addChild(sUITransform* child)
{
    m_children.insert(child);
}

void sUITransform::deleteChild(sUITransform* child)
{
    m_children.erase(child);
}

void sUITransform::setParent(sUITransform* parent)
{
    if (parent != m_parent)
    {
        m_parent->deleteChild(this);
        m_parent = parent;
        m_parent->addChild(this);
        m_dirty = true;
        EntitySystem::Get()->onUITransformChanged(this);
    }
}

void sUITransform::UpdateModelMatriceRecursively()
{
    if (!m_dirty)
        return;

    m_dirty = false;
    m_globalPosition = m_position;

    glm::mat4 parentModel(1.0f);
    if (m_parent)
    {
        parentModel = m_parent->get_modelMatrice();
        m_globalPosition += m_parent->getGlobalPosition();
    }

    m_modelMatrice = glm::mat4(1.0f);
    m_modelMatrice = glm::scale(m_modelMatrice, m_size);
    m_modelMatrice = glm::rotate(m_modelMatrice, glm::radians(m_rotation.x), glm::vec3(1,0,0));
    m_modelMatrice = glm::rotate(m_modelMatrice, glm::radians(m_rotation.y), glm::vec3(0,1,0));
    m_modelMatrice = glm::rotate(m_modelMatrice, glm::radians(m_rotation.z), glm::vec3(0,0,1));
    m_modelMatrice = glm::translate(m_modelMatrice, m_position);

    m_modelMatrice *= parentModel;

    for (auto& child : m_children)
    {
        child->UpdateModelMatriceRecursively();
    }
}
