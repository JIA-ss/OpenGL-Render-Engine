#include "Component.h"
#include "graphic/Mesh.h"

#include <functional>

class RenderSystem;
namespace Render { 
    class RenderQueue;
}

COMPONENT_NAMESPACE_BEGIN


class sMeshRender : public sComponent
{
    friend class RenderSystem;
    friend class Render::RenderQueue;
    COMPONENT_DECLARE
    COMPONENT_PROPERTY(unsigned int, renderIdx, 1)
    COMPONENT_PROPERTY(unsigned int, renderQueueOrder, 2000)
    COMPONENT_PROPERTY(std::vector<Graphic::Mesh*>, meshes, {})
    COMPONENT_PROPERTY(std::vector<std::vector<Graphic::Material*>>, respectiveMaterials, {})
    COMPONENT_PROPERTY(std::vector<Graphic::Material*>, sharedMaterials, {})
    COMPONENT_PROPERTY_WITHOUT_COMPARE(std::vector<std::function<void()>>, passOperations, {})
public:
    sMeshRender(Graphic::Mesh* mesh, unsigned int order = 2000) : m_meshes({mesh}), m_renderQueueOrder(order) { }
    sMeshRender(const std::vector<Graphic::Mesh*>& meshes, unsigned int order = 2000) : m_meshes(meshes), m_renderQueueOrder(order) { }
    void OnEnable() override;
    void OnDisable() override;
public:
    bool isValid() const;
    void set_passOperation(unsigned int idx, const std::function<void()>& func);
    int get_pass() const;
private:
    void draw() const;
    void once_draw(Graphic::Shader* shader);
};

COMPONENT_NAMESPACE_END