#pragma once
#include "RenderCommonHeader.h"
#include "graphic/Model.h"

#include <map>
#include <set>
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

    void EnqueMesh(Graphic::Mesh* mesh, unsigned int order = (unsigned int)Order::Geometry);
    void EnqueModel(Graphic::Model* model, unsigned int order = (unsigned int)Order::Geometry);

    void Render(Order order, Graphic::Shader* shader = nullptr);
private:
    struct RenderElement
    {
        Graphic::Mesh* mesh;
        RenderElement(Graphic::Mesh* _mesh) : mesh(_mesh) { }
        bool operator<(const RenderElement& re) const { return mesh->GetRenderingIndex() < re.mesh->GetRenderingIndex(); }
    };

    typedef std::multiset<RenderElement> RenderSet;

    Order GetTargetOrder(unsigned int order);
    std::map<unsigned int, RenderSet>& GetTargetOrderQueue(Order order);
private:
    std::map<unsigned int, RenderSet> m_backgrounds;
    std::map<unsigned int, RenderSet> m_geometries;
    std::map<unsigned int, RenderSet> m_transparents;
    std::map<unsigned int, RenderSet> m_overlays;
    glm::vec3 m_cameraPos;
};

RENDER_NAMESPACE_END