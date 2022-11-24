#include "mainscript.h"

#include "system/RenderSystem.h"
#include "system/ResourceSystem.h"

#include "entity/Camera.h"
#include "entity/Entity.h"
#include "entity/GameObject.h"

#include "component/Transform.h"
#include "component/MeshRender.h"

#include "graphic/Model.h"

#include <glm/glm.hpp>

GRAPHIC_NAMESPACE_USING
ENTITY_NAMESPACE_USE
COMPONENT_NAMESPACE_USING

void generatePlane()
{
    ZoneScopedN("mainscript generatePlane")
    RenderSystem* rdSystem = RenderSystem::Get();
    
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);
    cam.setCameraPos(glm::vec3(-2.6982f,2.36253f,6.1544f));
    cam.setPitch(-45);
    cam.setYaw(-15);

    sEntity* plane = sEntity::Create<sGameObject>("plane");
    sTransform* t = plane->GetComponent<sTransform>();
    t->set_rotation(glm::vec3(90,0,0), true);
    t->set_size(glm::vec3(10,0.01,10));
    t->set_position(glm::vec3(0.0f));
    

    Texture* diff = ResourceSystem::LoadGraphicResource<Texture>("plane-dif-tex", "Wall/wall.jpg", Diffuse).GetGraphic();
    Texture* shadow = rdSystem->GetForwardRendering().GetShadowMapingController().GetDepthTexture();
    Mesh* planeMesh = ResourceSystem::LoadGraphicResource<Mesh>("plane-Mesh", VertexStream::box, "ShadowMapping/PCSS", std::vector<Texture*>{shadow, diff}).GetGraphic();
    plane->AddComponent<sMeshRender>(std::vector<Mesh*>{planeMesh});

    
}



void mainscript()
{
    ZoneScopedN("mainscript")
    generatePlane();

    std::vector<glm::vec3> objectPositions = 
    {
        glm::vec3(-3.0,0.45, -3.0),
        glm::vec3(0.0, 0.45, -3.0),
        glm::vec3(3.0, 0.45, -3.0),
        glm::vec3(-3.0,0.45, 0.0),
        glm::vec3(0.0, 0.45, 0.0),
        glm::vec3(3.0, 0.45, 0.0),
        glm::vec3(-3.0,0.45, 3.0),
        glm::vec3(0.0, 0.45, 3.0),
        glm::vec3(3.0, 0.45, 3.0)
    };

    Model* model = ResourceSystem::LoadGraphicResource<Model>("model", "aircraft/E 45 Aircraft_obj.obj", "ShadowMapping/ModelWithShadow").GetGraphic();
    for (int i = 0; i < objectPositions.size(); i++ )
    {
        sEntity* entity = sEntity::Create<sGameObject>("model" + std::to_string(i));
        entity->AddComponent<sMeshRender>(model->GetMeshes());
        sTransform* transform = entity->GetComponent<sTransform>();
        transform->set_position(objectPositions[i] * 3.0f);
        transform->set_size(glm::vec3(0.25f));
    }
}