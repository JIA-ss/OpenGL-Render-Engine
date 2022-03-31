#include "graphicTest.h"
#include "graphic/Texture.h"
#include "graphic/CubeMap.h"
#include "system/RenderSystem.h"
#include "system/ResourceSystem.h"
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

    //Texture* defaultTexture = new Texture("Blend/plane.png",Diffuse);
    //Material* mat = new Material("ShadowMapping/ShadowPass", {defaultTexture});
    //Mesh* mesh = new Mesh(Vertex::boxElement, Vertex::box, mat, "mesh");

    auto& forward = rdSystem->GetForwardRendering();
    Texture* diffuseTex = ResourceSystem::LoadGraphicResource<Texture>("Blend/plane.png- Texture", "Blend/plane.png", TextureType::Diffuse).GetGraphic();
    Texture* depthTex = forward.GetShadowMapingController().GetDepthTexture();
    Texture* depthAttachTest = ResourceSystem::GetGraphic<Texture>("DepthAttachment- Texture");


    GraphicResource _mesh_ = ResourceSystem::LoadGraphicResource<Mesh>("firstMesh", VertexStream::box, "ShadowMapping/ShadowPass", std::vector<Texture* >{diffuseTex/*, depthTex*/});
    Mesh* mesh = _mesh_.GetGraphic();

    sGameObject* obj = (sGameObject*)sEntity::Create<sGameObject>("default");
    obj->AddComponent<sMeshRender>(mesh);
    obj->GetComponent<sTransform>()->set_position(glm::vec3(2,0.5,-3));

    sEntity* cube = sEntity::Create<sGameObject>("cube");
    cube->AddComponent<sMeshRender>(mesh);
    sTransform* t = cube->GetComponent<sTransform>();
    t->set_position(glm::vec3(1,2,-2));


    sEntity* plane = sEntity::Create<sGameObject>("plane");
    sTransform* p_t = plane->GetComponent<sTransform>();
    plane->AddComponent<sMeshRender>(mesh);
    p_t->set_size(glm::vec3(100,0.5,100));
    p_t->set_position(glm::vec3(0,-1,0));


    //Graphic::CubeMap* cubeMap = new CubeMap("skybox/", ".jpg");
    Graphic::CubeMap* cubeMap = ResourceSystem::LoadGraphicResource<CubeMap>("sky", "skybox/", ".jpg").GetGraphic();
    Mesh* sky = ResourceSystem::LoadGraphicResource<Mesh>("sky", VertexStream::box, "CubeMap/skybox", std::vector<Texture*>{cubeMap}).GetGraphic();
    
    sEntity* entity = sEntity::Create<sEntity>("skybox");
    entity->AddComponent<sMeshRender>(sky, 1000);
}

void GraphicTest::_model_forward_()
{
    RenderSystem* rdSystem = RenderSystem::Get();
    
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);

    std::vector<glm::vec3> objectPositions = 
    {
        glm::vec3(-3.0, -3.0, -3.0),
        glm::vec3(0.0, -3.0, -3.0),
        glm::vec3(3.0, -3.0, -3.0),
        glm::vec3(-3.0, -3.0, 0.0),
        glm::vec3(0.0, -3.0, 0.0),
        glm::vec3(3.0, -3.0, 0.0),
        glm::vec3(-3.0, -3.0, 3.0),
        glm::vec3(0.0, -3.0, 3.0),
        glm::vec3(3.0, -3.0, 3.0)
    };

    Model* model = ResourceSystem::LoadGraphicResource<Model>("model", "aircraft/E 45 Aircraft_obj.obj").GetGraphic();
    for (int i = 0; i < objectPositions.size(); i++ )
    {
        sEntity* entity = sEntity::Create<sGameObject>("model" + std::to_string(i));
        entity->AddComponent<sMeshRender>(model->GetMeshes());
        sTransform* transform = entity->GetComponent<sTransform>();
        transform->set_position(objectPositions[i] * 3.0f);
        transform->set_size(glm::vec3(0.25f));
    }
}

void GraphicTest::_model_deferred_()
{
    GlobalShaderParam* gs = GlobalShaderParam::Get();
    std::vector<glm::vec3> lightPos;
    std::vector<glm::vec3> lightColor;

    for (int i = 0; i < 5; i++)
    {
        // Calculate slightly random offsets
        GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
        GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        lightPos.push_back(glm::vec3(xPos * 2, yPos * 2, zPos * 2));
        // Also calculate random color
        GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        lightColor.push_back(glm::vec3(rColor, gColor, bColor));
    }

    for (int i = 0; i < 5; i++)
    {
        gs->SubData("LightPositions", i * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos[i]));
        gs->SubData("LightColors", i * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightColor[i]));
    }
    
    _model_forward_();
}