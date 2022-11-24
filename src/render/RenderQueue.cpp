#include "RenderQueue.h"
#include "component/MeshRender.h"
#include "component/Transform.h"

#include "system/RenderSystem.h"
#include <Tracy.hpp>
RENDER_NAMESPACE_USING

bool RenderQueue::RenderElement::operator<(const RenderQueue::RenderElement& re) const 
{ 
    return mesh < re.mesh; 
}

RenderQueue::TransparentRenderElement::TransparentRenderElement(Component::sMeshRender* _mesh)
{
    ZoneScopedN("RenderQueue::TransparentRenderElement::TransparentRenderElement");
    glm::vec3 cam_pos = RenderSystem::Get()->getCamera().getCameraPos();
    mesh = _mesh;
    Component::sTransform* trans =  mesh->get_entity()->GetComponent<Component::sTransform>();
    distance = glm::distance(cam_pos, trans->getGlobalPosition());
}

bool RenderQueue::TransparentRenderElement::operator<(const TransparentRenderElement& re) const
{
    return distance >= re.distance;
}

RenderQueue::Order RenderQueue::GetTargetOrder(unsigned int order)
{
    if (order < (unsigned int)Order::Geometry)
        return Order::Background;
    if (order < (unsigned int)Order::Transparent)
        return Order::Geometry;
    if (order < (unsigned int)Order::Overlay)
        return Order::Transparent;
    return Order::Overlay;
}

std::map<unsigned int, RenderQueue::RenderSet>& RenderQueue::GetTargetOrderQueue(Order order)
{
    switch (order)
    {
    case Order::Background :
        return m_backgrounds;
    case Order::Geometry:
        return m_geometries;
    case Order::Transparent:
        return m_transparents;
    case Order::Overlay:
        return m_overlays;
    default:
        break;
    }
    static std::map<unsigned int, RenderSet> invalid{};
    return invalid;
}

void RenderQueue::Enqueue(Component::sMeshRender* meshRender,unsigned int order)
{
    ZoneScopedN("RenderQueue::Enqueue");
    auto& targetQue = GetTargetOrderQueue(GetTargetOrder(order));
    RenderElement element(meshRender);
    targetQue[order].insert(element);
}
void RenderQueue::Dequeue(Component::sMeshRender* meshRender,unsigned int order)
{
    ZoneScopedN("RenderQueue::Dequeue");
    auto& targetQue = GetTargetOrderQueue(GetTargetOrder(order));
    RenderElement element(meshRender);
    auto it = targetQue[order].find(element);
    if (it != targetQue[order].end())
        targetQue[order].erase(element);
}


void RenderQueue::Render(Order order, Graphic::Shader* shader)
{
    ZoneScopedN("RenderQueue::Render");
    if (order == Transparent)
    {
        RenderTransparentObj(shader);
    }
    else
    {
        ZoneScopedN("RenderQueue::Render Scene");
        if (order == Order::Background)
            glDepthFunc(GL_LEQUAL);
        
        auto& targetQue = GetTargetOrderQueue(order);
        for (auto&&[renderIdx, renderSet] : targetQue)
        {
            for (auto&& element : renderSet)
            {
                if (shader)
                    element.mesh->once_draw(shader);
                else
                    element.mesh->draw();
            }
        }

        if (order == Order::Background)
            glDepthFunc(GL_LESS);  
    }
}

void RenderQueue::RenderTransparentObj(Graphic::Shader* shader)
{
    ZoneScopedN("RenderQueue::RenderTransparentObj");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    std::set<TransparentRenderElement> renderQueue;
    for (auto&& [invalid, elements] : m_transparents)
    {
        for (auto& re : elements)
        {
            renderQueue.insert(re.mesh);
        }
    }

    for (auto it = renderQueue.begin(); it != renderQueue.end(); it++)
    {
        if (shader)
            it->mesh->once_draw(shader);
        else
            it->mesh->draw();
    }
}