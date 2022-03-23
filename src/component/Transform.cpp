#include "Transform.h"
#include "entity/Entity.h"
#include "system/EntitySystem.h"

COMPONENT_NAMESPACE_USING

COMPONENT_IMPLEMENT(sTransform)

void sTransform::notifyPropertyChanged(const prop_variant& variant)
{
    m_modelMatriceDirty = true;
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
    //EntitySystem::Get()->onParentChanged(clone, nullptr);
    return clone;
}