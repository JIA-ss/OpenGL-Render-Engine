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

void GraphicTest::_3d(Window* window)
{
    window->enableZTest(true);
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.enableTarget(false);
    cam.setSensitive(0.3f);

    std::vector<Vertex> vertexs = {
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f), glm::vec3(0.0f,  1.0f,  0.0f)}
    };

    glm::vec3 lightPos = glm::vec3(10,0,0);
    glm::vec3 ambientColor = glm::vec3(1.0f) * 0.5f;
    glm::vec3 lightColor = glm::vec3(1.0f);

    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalProjMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));
    gsp->SubData("GlobalPositions", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos));

    gsp->GenBlock("GlobalColors", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalColors", 0, sizeof(glm::vec3), glm::value_ptr(ambientColor));
    gsp->SubData("GlobalColors", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightColor));
    
    Material* cubeMat = new Material("graphic_3d_cube", {});
    cubeMat->SetShaderParam("model", glm::mat4(1.0f));
    cubeMat->SetShaderParam("objectColor", glm::vec3( 1.0f, 0.5f, 0.31f));

    glm::mat4 model(1.0f);
    model = glm::translate(model, lightPos);
    Material* lightMat = new Material("graphic_3d_light", {});
    lightMat->SetShaderParam("model", model);

    Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMat, "cube");
    Mesh* light = new Mesh(Vertex::boxElement, Vertex::box, lightMat, "light");
    window->AddUpdateCallback([window, cube, light](){
        Camera& cam = window->getCamera();
        GlobalShaderParam* gsp = GlobalShaderParam::Get();

float radius = 10.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;
glm::mat4 view;
view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); 

        //view = cam.getViewMat4();
        gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

        gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));
        //gsp->SubData("GlobalPositions", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos));

        light->draw();
        cube->draw();
    });
}

void GraphicTest::_model(Window* window)
{
    window->enableZTest(true);
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.enableTarget(false);
    cam.setSensitive(0.01f);

    glm::vec3 lightPos = glm::vec3(10,0,0);
    glm::vec3 ambientColor = glm::vec3(1.0f) * 0.5f;
    glm::vec3 lightColor = glm::vec3(1.0f);


    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalProjMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));
    gsp->SubData("GlobalPositions", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos));

    gsp->GenBlock("GlobalColors", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalColors", 0, sizeof(glm::vec3), glm::value_ptr(ambientColor));
    gsp->SubData("GlobalColors", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightColor));


    Material* cubeMat = new Material("graphic_model", {});
    cubeMat->SetShaderParam("model", glm::mat4(1.0f));

    glm::mat4 model(1.0f);
    model = glm::translate(model, lightPos);
    Material* lightMat = new Material("graphic_3d_light", {});
    lightMat->SetShaderParam("model", model);


    Mesh* light = new Mesh(Vertex::boxElement, Vertex::box, lightMat, "light");
    Model* _model = new Model("nanosuit/nanosuit.obj");
    
    glm::mat4 model_modelTrans(1.0f);
    model_modelTrans = glm::translate(model_modelTrans, glm::vec3(0,-4,1));
    model_modelTrans = glm::scale(model_modelTrans, glm::vec3(0.4,0.4,0.4));
    _model->SetShaderParam("model", model_modelTrans);
    window->AddUpdateCallback([window, _model, light](){
        Camera& cam = window->getCamera();
        GlobalShaderParam* gsp = GlobalShaderParam::Get();

        float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //glm::mat4 view;
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); 

        glm::mat4 view = cam.getViewMat4();
        gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

        gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));
        //gsp->SubData("GlobalPositions", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos));

        glm::mat4 model_modelTrans(1.0f);
        //model_modelTrans = glm::translate(model_modelTrans, glm::vec3(0,-4,1));
        //model_modelTrans = glm::rotate(model_modelTrans, camX, glm::vec3(0,1,0));
        model_modelTrans = glm::translate(model_modelTrans, glm::vec3(0,-4,0));
        model_modelTrans = glm::scale(model_modelTrans, glm::vec3(0.2,0.2,0.2));
        _model->SetShaderParam("model", model_modelTrans);

        light->draw();
        _model->draw();
    });
}

void GraphicTest::_depthTest(Window* window)
{
    window->enableZTest(true);
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.03f);

    glm::vec3 lightPos = glm::vec3(10,0,0);
    glm::vec3 ambientColor = glm::vec3(1.0f) * 0.5f;
    glm::vec3 lightColor = glm::vec3(1.0f);

    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalProjMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));
    gsp->SubData("GlobalPositions", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos));

    gsp->GenBlock("GlobalColors", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalColors", 0, sizeof(glm::vec3), glm::value_ptr(ambientColor));
    gsp->SubData("GlobalColors", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightColor));
    
    std::vector<Mesh*> cubes;
    for (int i = 0; i < 10; i++)
    {
        Material* cubeMat = new Material("DepthTest/depth_output", {});
        glm::vec3 cubePos = glm::vec3(i,i,-i) * 0.1f;
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePos);
        cubeMat->SetShaderParam("model", model);
        cubeMat->SetShaderParam("objectColor", glm::vec3( 1.0f, 0.5f + i * 0.05f, 0.31f));
        Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMat, "cube");

        cubes.push_back(cube);
    }

    glm::mat4 model(1.0f);
    model = glm::translate(model, lightPos);
    Material* lightMat = new Material("graphic_3d_light", {});
    lightMat->SetShaderParam("model", model);
    Mesh* light = new Mesh(Vertex::boxElement, Vertex::box, lightMat, "light");
    window->AddUpdateCallback([window, cubes, light](){
        Camera& cam = window->getCamera();
        GlobalShaderParam* gsp = GlobalShaderParam::Get();
        glm::mat4 view = cam.getViewMat4();
        gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));
        gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

        light->draw();
        for (auto& cube : cubes)
        {
            cube->draw();
        }
    });
}

void GraphicTest::_stencilTest(Window* window)
{
    window->enableZTest(true);
    window->enableStencil(true);

    Render::StencilTest& st = window->getStencilTest();
    st.SetFunc(Render::StencilTest::NotEqual, 1);
    st.SetOp(Render::StencilTest::Keep, Render::StencilTest::Keep, Render::StencilTest::Replace);
    st.EnableWrite(true);

    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.01f);

    glm::vec3 lightPos = glm::vec3(10,0,0);
    glm::vec3 ambientColor = glm::vec3(1.0f) * 0.5f;
    glm::vec3 lightColor = glm::vec3(1.0f);


    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalProjMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));
    gsp->SubData("GlobalPositions", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightPos));

    gsp->GenBlock("GlobalColors", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalColors", 0, sizeof(glm::vec3), glm::value_ptr(ambientColor));
    gsp->SubData("GlobalColors", sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(lightColor));


    Material* cubeMat = new Material("graphic_model", {});
    cubeMat->SetShaderParam("model", glm::mat4(1.0f));

    glm::mat4 model(1.0f);
    model = glm::translate(model, lightPos);
    Material* lightMat = new Material("graphic_3d_light", {});
    lightMat->SetShaderParam("model", model);


    Mesh* light = new Mesh(Vertex::boxElement, Vertex::box, lightMat, "light");
    Model* _model = new Model("nanosuit/nanosuit.obj", "StencilTest/outline");
    
    glm::mat4 model_modelTrans(1.0f);
    model_modelTrans = glm::translate(model_modelTrans, glm::vec3(0,-4,1));
    model_modelTrans = glm::scale(model_modelTrans, glm::vec3(0.4,0.4,0.4));
    _model->SetShaderParam("model", model_modelTrans);
    window->AddUpdateCallback([window, _model, light](){
        Camera& cam = window->getCamera();
        GlobalShaderParam* gsp = GlobalShaderParam::Get();
        auto& st = window->getStencilTest();
        glm::mat4 view = cam.getViewMat4();
        gsp->SubData("GlobalProjMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalProjMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

        gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

        st.EnableWrite(false);
        st.SetUp();
        //glStencilMask(0x00);
        light->draw();


        st.EnableWrite(true);
        st.SetFunc(Render::StencilTest::Always, 1);
        st.SetUp();
        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);

        _model->SetShaderParam("drawOutline", 0);
        glm::mat4 model_modelTrans(1.0f);
        model_modelTrans = glm::mat4(1.0f);
        model_modelTrans = glm::translate(model_modelTrans, glm::vec3(4,-4,0));
        model_modelTrans = glm::scale(model_modelTrans, glm::vec3(0.5,0.5,0.5));
        _model->SetShaderParam("model", model_modelTrans);
        _model->draw();

        st.EnableWrite(true);
        st.SetFunc(Render::StencilTest::NotEqual, 1);
        st.SetUp();
        //glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glStencilMask(0xFF);
        model_modelTrans = glm::mat4(1.0f);
        model_modelTrans = glm::translate(model_modelTrans, glm::vec3(4,-4,0));
        model_modelTrans = glm::scale(model_modelTrans, glm::vec3(0.5,0.5,0.5));
        _model->SetShaderParam("model", model_modelTrans);
        _model->SetShaderParam("drawOutline", 1);
        _model->SetShaderParam("outlineColor", glm::vec3(1.0f, 0.5f, 0.31f));
        _model->SetShaderParam("outlineWidth", 0.1f);
        _model->draw();


        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
    });
}