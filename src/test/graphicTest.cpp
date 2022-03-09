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