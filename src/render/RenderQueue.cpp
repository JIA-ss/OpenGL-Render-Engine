#include "RenderQueue.h"
#include "component/MeshRender.h"
RENDER_NAMESPACE_USING

bool RenderQueue::RenderElement::operator<(const RenderQueue::RenderElement& re) const 
{ 
    return mesh < re.mesh; 
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
    return std::map<unsigned int, RenderSet>{};
}

void RenderQueue::Enqueue(Component::sMeshRender* meshRender,unsigned int order)
{
    auto& targetQue = GetTargetOrderQueue(GetTargetOrder(order));
    RenderElement element(meshRender);
    targetQue[order].insert(element);
}
void RenderQueue::Dequeue(Component::sMeshRender* meshRender,unsigned int order)
{
    auto& targetQue = GetTargetOrderQueue(GetTargetOrder(order));
    RenderElement element(meshRender);
    auto it = targetQue[order].find(element);
    if (it != targetQue[order].end())
        targetQue[order].erase(element);
}


void RenderQueue::Render(Order order, Graphic::Shader* shader)
{
    if (order == Order::Background)
        glDepthFunc(GL_LEQUAL);

    // todo: sort by distance with camera when order is transparent
    auto& targetQue = GetTargetOrderQueue(order);
    for (auto&&[renderIdx, renderSet] : targetQue)
    {
        for (auto&& element : renderSet)
        {
            element.mesh->once_draw(shader);
        }
    }

    if (order == Order::Background)
        glDepthFunc(GL_LESS);  
}