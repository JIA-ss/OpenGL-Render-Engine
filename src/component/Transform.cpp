#include "Transform.h"
#include "entity/Entity.h"
#include "system/EntitySystem.h"

COMPONENT_NAMESPACE_USING

COMPONENT_IMPLEMENT(sTransform)

void sTransform::notifyPropertyChanged(const prop_variant& variant)
{
    m_modelMatriceDirty = true;
    EntitySystem::Get()->onTransformChanged(this);
}

bool sTransform::deleteChild(sTransform* child)
{
    auto it = m_children.find(child);
    if (it == m_children.end())
        return false;
    m_children.erase(it);
    return true;
}

void sTransform::addChild(sTransform* child)
{
    m_children.insert(child);
}

void sTransform::setParent(sTransform* parent)
{
    sTransform* oldParent = m_parent;
    m_parent = parent;
    parent->addChild(this);
    m_modelMatriceDirty = true;
    EntitySystem::Get()->onParentChanged(this, oldParent);
    EntitySystem::Get()->onTransformChanged(this);
}

std::set<sTransform*> sTransform::getChildren() const
{
    return m_children;
}
int sTransform::getChildrenSize() const
{
    return m_children.size();
}
sTransform* sTransform::getParent()
{
    return m_parent;
}

void sTransform::OnDestroy()
{
    for (auto child : m_children)
    {
        Entity::sEntity::Destroy(child->get_entity());
    }
}

sComponent* sTransform::Clone()
{
    sTransform* clone = (sTransform*)sComponent::Clone();
    clone->m_children.clear();
    clone->m_parent = nullptr;
    clone->m_modelMatriceDirty = true;
    EntitySystem::Get()->onTransformChanged(this);
    return clone;
}

void sTransform::UpdateModelMatriceRecursively()
{
    if (!m_modelMatriceDirty)
        return;
    m_modelMatriceDirty = false;
    m_globalPosition = m_position;

    glm::mat4 parentModel(1.0f);
    if (m_parent)
    {
        parentModel = m_parent->get_modelMatrice();
        m_globalPosition += m_parent->getGlobalPosition();
    }

    m_modelMatrice = glm::mat4(1.0f);
    m_modelMatrice = glm::translate(m_modelMatrice, m_position);
    m_modelMatrice = glm::rotate(m_modelMatrice, m_rotation.x, glm::vec3(1,0,0));
    m_modelMatrice = glm::rotate(m_modelMatrice, m_rotation.y, glm::vec3(0,1,0));
    m_modelMatrice = glm::rotate(m_modelMatrice, m_rotation.z, glm::vec3(0,0,1));
    m_modelMatrice = glm::scale(m_modelMatrice, m_size);

    m_modelMatrice *= parentModel;

    for (auto& child : m_children)
    {
        child->UpdateModelMatriceRecursively();
    }
}