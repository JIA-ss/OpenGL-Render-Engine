#include "graphicTest.h"
#include "graphic/Texture.h"
#include "graphic/CubeMap.h"
#include "system/RenderSystem.h"
#include "component/Transform.h"
#include "entity/Entity.h"
#include <glm/vec4.hpp>
#include <vector>
using namespace GraphicTest;
using namespace Graphic;


void GraphicTest::_shadowMapping_engine()
{
    RenderSystem* rdSystem = RenderSystem::Get();
    auto& rq = rdSystem->getRenderQueue();
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);
    rdSystem->getShadowMapping().SetActive(true);
    rdSystem->getShadowMapping().Init();
    Texture* depthTexture = rdSystem->getShadowMapping().GetDepthTexture();

    glm::vec3 planePos = glm::vec3(0,-0.5,0);
    glm::vec3 planeSize = glm::vec3(100,0.1,100);

    Texture* planeTex = new Texture("Blend/plane.png", Diffuse);
    Material* planeMaterial = new Material("ShadowMapping/ShadowPass", {depthTexture, planeTex});
    Mesh* plane = new Mesh(Vertex::boxElement, Vertex::box, planeMaterial, "plane");
    plane->SetPosition(planePos);
    plane->SetSize(planeSize);
    rq.EnqueMesh(plane);

    std::vector<glm::vec3> cubePoses = {
        glm::vec3(0,4,0),
        glm::vec3(3,0.5,0),
        glm::vec3(-1,2,-1)
    };

    Texture* cubeTex = new Texture("Blend/cube.jpg", Diffuse);
    Material* cubeMaterial = new Material("ShadowMapping/ShadowPass", {depthTexture, cubeTex});
    Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMaterial, "cube");
    for (int i = 0; i < cubePoses.size(); i++)
    {
        cube->SetPosition(cubePoses[i]);
        rq.EnqueMesh(cube);
        cube = cube->Clone();
    }
}

void GraphicTest::_cube_map_test_()
{
    RenderSystem* rdSystem = RenderSystem::Get();
    auto& rq = rdSystem->getRenderQueue();
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);


    Graphic::CubeMap* cubeMap = new CubeMap("skybox/", ".jpg");
    Material* skybox = new Material("CubeMap/skybox", { cubeMap });
    Mesh* sky = new Mesh(Vertex::boxElement, Vertex::box, skybox, "skybox");
    rq.EnqueMesh(sky, 2);


    _shadowMapping_engine();
}

void GraphicTest::_component_test()
{
    Entity::sEntity* entity = Entity::sEntity::Create<Entity::sEntity>();
    Component::sTransform* transfrom = entity->AddComponent<Component::sTransform>();
    transfrom->set_position(glm::vec3(5.0f));

    Entity::sEntity* newEntity = Entity::sEntity::Clone(entity);
    Component::sTransform* newTransform = newEntity->GetComponent<Component::sTransform>();

    std::cout << newTransform->get_position().x << std::endl;

}