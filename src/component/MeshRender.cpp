#include "MeshRender.h"
#include "Transform.h"


#include "system/RenderSystem.h"

COMPONENT_NAMESPACE_USING

COMPONENT_IMPLEMENT(sMeshRender)

bool sMeshRender::isValid() const
{
    return !m_meshes.empty();
}

int sMeshRender::get_pass() const
{
    if (!isValid())
        return 0;
    int pass = m_sharedMaterials.size() > m_respectiveMaterials.size() ? m_sharedMaterials.size() : m_respectiveMaterials.size();
    return pass > 0 ? pass : 1;
}

void sMeshRender::draw() const 
{
    if (!isValid())
        return;
    
    int passes = get_pass();

    glm::mat4 model(1.0f);
    sTransform* transform = get_entity()->GetComponent<sTransform>();
    if (transform)
        model = transform->get_modelMatrice();

    for (int meshIdx = 0; meshIdx < m_meshes.size(); meshIdx++)
    {
        for (int pass = 0; pass < passes; pass++)
        {
            if (pass < m_passOperations.size())
                m_passOperations[pass]();
            
            // use shared materials
            if (!m_sharedMaterials.empty())
            {
                if (pass < m_sharedMaterials.size())
                {
                    m_sharedMaterials[pass]->SetShaderParam("model", model);
                    m_meshes[meshIdx]->draw(m_sharedMaterials[pass]);
                }
                else
                {
                    m_meshes[meshIdx]->SetShaderParam("model", model);
                    m_meshes[meshIdx]->draw();
                }
            }
            else 
            {
                // use respective materials
                if (pass < m_respectiveMaterials.size() && meshIdx < m_respectiveMaterials[pass].size())
                {
                    m_respectiveMaterials[pass][meshIdx]->SetShaderParam("model", model);
                    m_meshes[meshIdx]->draw(m_respectiveMaterials[pass][meshIdx]);
                }
                else
                {
                    m_meshes[meshIdx]->SetShaderParam("model", model);
                    m_meshes[meshIdx]->draw();
                }
            }
        }
    }
}


void sMeshRender::once_draw(Graphic::Shader* shader)
{
    if (!isValid())
        return;
    glm::mat4 model(0.0f);
    sTransform* transform = get_entity()->GetComponent<sTransform>();
    if (transform)
        model = transform->get_modelMatrice();
    for (auto& mesh : m_meshes)
    {
        mesh->SetShaderParam("model", model);
        mesh->draw(shader);
    }
}


void sMeshRender::OnEnable()
{
    auto& rq = RenderSystem::Get()->getRenderQueue();
    rq.Enqueue(this, m_renderQueueOrder);
    Entity::sEntity* entity = get_entity();
    sTransform* transform = entity->GetComponent<sTransform>();
    if (transform)
    {
        auto children = transform->getChildren();
        for (auto& child : children)
        {
            Entity::sEntity* childEntity = child->get_entity();
            sMeshRender* childMR = childEntity->GetComponent<sMeshRender>();
            if (childMR && childEntity->IsActiveSelf())
                childMR->OnEnable();
        }
    }
}

void sMeshRender::OnDisable()
{
    auto& rq = RenderSystem::Get()->getRenderQueue();
    rq.Dequeue(this, m_renderQueueOrder);
    Entity::sEntity* entity = get_entity();
    sTransform* transform = entity->GetComponent<sTransform>();
    if (transform)
    {
        auto children = transform->getChildren();
        for (auto& child : children)
        {
            Entity::sEntity* childEntity = child->get_entity();
            sMeshRender* childMR = childEntity->GetComponent<sMeshRender>();
            if (childMR)
                childMR->OnDisable();
        }
    }
}
