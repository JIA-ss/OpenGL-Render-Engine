#include "RenderQueue.h"

RENDER_NAMESPACE_USING


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


void RenderQueue::EnqueMesh(Graphic::Mesh* mesh, unsigned int order)
{
    auto& targetQue = GetTargetOrderQueue(GetTargetOrder(order));
    targetQue[order].emplace(mesh);
}

void RenderQueue::EnqueModel(Graphic::Model* model, unsigned int order)
{
    auto& targetQue = GetTargetOrderQueue(GetTargetOrder(order));
    auto& meshes = model->GetMeshes();
    for (auto& mesh : meshes)
    {
        targetQue[order].emplace(mesh);
    }
}


void RenderQueue::Render(Order order, Graphic::Shader* shader)
{
    // todo: sort by distance with camera when order is transparent
    auto& targetQue = GetTargetOrderQueue(order);
    for (auto&&[renderIdx, renderSet] : targetQue)
    {
        for (auto&& element : renderSet)
        {
            element.mesh->draw(shader);
        }
    }
}