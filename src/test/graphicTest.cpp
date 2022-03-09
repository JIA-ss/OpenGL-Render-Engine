#include "graphicTest.h"
#include "graphic/Texture.h"
#include "pipline/Shader.h"
#include <glm/vec4.hpp>
#include <vector>
using namespace GraphicTest;
using namespace Graphic;
void GraphicTest::simpleImage(Window* window)
{
    //window->enableZTest(true);
    Texture* tex1 = new Texture("container.jpg");
    Texture* tex2 = new Texture("G_Table_Wood_10.png");

    Material* material = new Material("graphic_texture", std::vector<Texture *>{tex2, tex1});
    bool v = material->SetShaderParam("aColor", glm::vec3(1,1,1));
    bool v2 = material->SetShaderParam("transform", glm::mat4(1.0f));

    Mesh* mesh = new Mesh(Vertex::quadElement, Vertex::quad, material, "test");

    window->AddUpdateCallback([mesh](){
        mesh->draw();
    });
}

void GraphicTest::cube(Window* window)
{
    window->enableZTest(true);
    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalProjMatrices", 2 * sizeof(glm::mat4), nullptr);
    Camera& cam = window->getCamera();
    cam.setCameraPos(glm::vec3(0,0,3));
    cam.enableControl(true);
    gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), &cam.getViewMat4()[0]);
    gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), &cam.getProjectionMat4()[0]);

    Texture* tex1 = new Texture("container.jpg");
    Texture* tex2 = new Texture("G_Table_Wood_10.png");
    Material* mat = new Material("graphic_cube", std::vector<Texture*>{tex1, tex2});
    mat->SetShaderParam("model", glm::mat4(1.0f));

    Mesh* mesh = new Mesh(Vertex::boxElement, Vertex::box, mat);

    window->AddUpdateCallback([window, gsp,mesh](){
        Camera& cam = window->getCamera();
        //std::cout << cam.getCameraPos().x << cam.getCameraPos().y<< cam.getCameraPos().z  << std::endl;
        gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), &cam.getViewMat4()[0]);
        gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), &cam.getProjectionMat4()[0]);

        mesh->draw();
    });
}