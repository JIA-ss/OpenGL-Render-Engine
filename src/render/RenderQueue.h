#pragma once
#include "RenderCommonHeader.h"
#include "graphic/Model.h"
#include <map>
#include <set>

namespace Component { class sMeshRender; }
RENDER_NAMESPACE_BEGIN


class RenderQueue
{
public:
    enum Order
    {
        Background = 1000,
        Geometry = 2000,
        Transparent = 3000,
        Overlay = 4000,
    };

    void Enqueue(Component::sMeshRender* meshRender,unsigned int order = (unsigned int)Order::Geometry);
    void Dequeue(Component::sMeshRender* meshRender,unsigned int order = (unsigned int)Order::Geometry);

    void Render(Order order, Graphic::Shader* shader = nullptr);
private:
    struct RenderElement
    {
        Component::sMeshRender* mesh;
        RenderElement(Component::sMeshRender* _mesh) : mesh(_mesh) { }
        bool operator<(const RenderElement& re) const;
    };

    struct TransparentRenderElement
    {
        Component::sMeshRender* mesh;
        unsigned int distance;
        TransparentRenderElement(Component::sMeshRender* _mesh);
        bool operator<(const TransparentRenderElement& re) const;
    };

    typedef std::set<RenderElement> RenderSet;

    Order GetTargetOrder(unsigned int order);
    std::map<unsigned int, RenderSet>& GetTargetOrderQueue(Order order);

    void RenderTransparentObj(Graphic::Shader* shader);
private:
    std::map<unsigned int, RenderSet> m_backgrounds;
    std::map<unsigned int, RenderSet> m_geometries;
    std::map<unsigned int, RenderSet> m_transparents;
    std::map<unsigned int, RenderSet> m_overlays;
    glm::vec3 m_cameraPos;
};

RENDER_NAMESPACE_END