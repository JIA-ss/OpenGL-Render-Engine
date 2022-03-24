#include "graphicTest.h"
#include "graphic/Texture.h"
#include "graphic/CubeMap.h"
#include "system/RenderSystem.h"
#include "component/Transform.h"
#include "component/MeshRender.h"
#include "entity/Entity.h"
#include "entity/GameObject.h"
#include <glm/vec4.hpp>
#include <vector>

using namespace Entity;
using namespace Component;
using namespace GraphicTest;
using namespace Graphic;


void GraphicTest::_shadowMapping_engine()
{
    RenderSystem* rdSystem = RenderSystem::Get();
    auto& rq = rdSystem->getRenderQueue();
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);
    //rdSystem->getShadowMapping().SetActive(true);
    //rdSystem->getShadowMapping().Init();
    //Texture* depthTexture = rdSystem->getShadowMapping().GetDepthTexture();

    glm::vec3 planePos = glm::vec3(0,-0.5,0);
    glm::vec3 planeSize = glm::vec3(100,0.1,100);

    Texture* planeTex = new Texture("Blend/plane.png", Diffuse);
    //Material* planeMaterial = new Material("ShadowMapping/RenderWithShadow", {depthTexture, planeTex});
    //Mesh* plane = new Mesh(Vertex::boxElement, Vertex::box, planeMaterial, "plane");
    //plane->SetPosition(planePos);
    //plane->SetSize(planeSize);
    //rq.EnqueMesh(plane);

    std::vector<glm::vec3> cubePoses = {
        glm::vec3(0,4,0),
        glm::vec3(3,0.5,0),
        glm::vec3(-1,2,-1)
    };

    Texture* cubeTex = new Texture("Blend/cube.jpg", Diffuse);
    //Material* cubeMaterial = new Material("ShadowMapping/RenderWithShadow", {depthTexture, cubeTex});
    //Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMaterial, "cube");
    for (int i = 0; i < cubePoses.size(); i++)
    {
        //cube->SetPosition(cubePoses[i]);
        //rq.EnqueMesh(cube);
        //cube = cube->Clone();
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
    //rq.EnqueMesh(sky, 2);


    _shadowMapping_engine();
}

void GraphicTest::_component_test()
{
    RenderSystem* rdSystem = RenderSystem::Get();
    
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);

    Texture* defaultTexture = new Texture("Blend/plane.png",Diffuse);
    Material* mat = new Material("ShadowMapping/ShadowPass", {defaultTexture});
    Mesh* mesh = new Mesh(Vertex::boxElement, Vertex::box, mat, "mesh");
    sGameObject* obj = (sGameObject*)sEntity::Create<sGameObject>("default");
    obj->AddComponent<sMeshRender>(mesh);

    sEntity* cube = sEntity::Create<sGameObject>("cube");
    cube->AddComponent<sMeshRender>(mesh);
    sTransform* t = cube->GetComponent<sTransform>();
    //t->setParent(obj->GetComponent<sTransform>());
    t->set_position(glm::vec3(1,2,-2));

    sEntity* plane = sEntity::Create<sGameObject>("plane");
    sTransform* p_t = plane->GetComponent<sTransform>();
    plane->AddComponent<sMeshRender>(mesh);
    p_t->set_size(glm::vec3(100,0.5,100));
    p_t->set_position(glm::vec3(0,-1,0));


    Graphic::CubeMap* cubeMap = new CubeMap("skybox/", ".jpg");
    Material* skybox = new Material("CubeMap/skybox", { cubeMap });
    Mesh* sky = new Mesh(Vertex::boxElement, Vertex::box, skybox, "skybox");
    sEntity* entity = sEntity::Create<sEntity>("skybox");
    entity->AddComponent<sMeshRender>(sky, 1000);
}