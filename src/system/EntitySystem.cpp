#include "EntitySystem.h"


void EntitySystem::Update()
{
    ZoneScopedN("EntitySystem::Update");
    auto it = m_dirtyTransforms.begin();
    while(it != m_dirtyTransforms.end())
    {
        if ((*it) != nullptr && (*it)->IsMatriceDirty())
        {
            (*it)->UpdateModelMatriceRecursively();
        }

        it = m_dirtyTransforms.erase(it);
    }
}

void EntitySystem::UnInit()
{
    ZoneScopedN("EntitySystem::UnInit");
    auto it = m_entities.begin();
    while (it != m_entities.end())
    {
        sEntity* entity = *it;
        it = m_entities.erase(it);
        sEntity::Destroy(entity);

    }

    m_roots.clear();
    m_dirtyTransforms.clear();
}

void EntitySystem::registerEntity(Entity::sEntity* entity)
{
    m_entities.insert(entity);
}

void EntitySystem::unregisterEntity(Entity::sEntity* entity)
{
    auto it_entity = m_entities.find(entity);
    if (it_entity != m_entities.end())
        m_entities.erase(it_entity);
    
    auto it_root = m_roots.find(entity);
    if (it_root != m_roots.end())
        m_roots.erase(it_root);
}

void EntitySystem::onParentChanged(sTransform* cur, sTransform* oldParent)
{
    if (!oldParent && cur->getParent())
    {
        m_roots.erase(cur->get_entity());
    }
    else if (!cur->getParent())
    {
        m_roots.insert(cur->get_entity());
    }
}

void EntitySystem::onTransformChanged(sTransform* trans)
{
    m_dirtyTransforms.insert(trans);
}
