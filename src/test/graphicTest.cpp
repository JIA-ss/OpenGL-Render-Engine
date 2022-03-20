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
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    Camera& cam = window->getCamera();
    cam.setCameraPos(glm::vec3(0,0,3));
    cam.enableControl(true);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), &cam.getViewMat4()[0]);
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), &cam.getProjectionMat4()[0]);

    Texture* tex1 = new Texture("container.jpg");
    Texture* tex2 = new Texture("G_Table_Wood_10.png");
    Material* mat = new Material("graphic_cube", std::vector<Texture*>{tex1, tex2});
    mat->SetShaderParam("model", glm::mat4(1.0f));

    Mesh* mesh = new Mesh(Vertex::boxElement, Vertex::box, mat);

    window->AddUpdateCallback([window, gsp,mesh](){
        Camera& cam = window->getCamera();
        //std::cout << cam.getCameraPos().x << cam.getCameraPos().y<< cam.getCameraPos().z  << std::endl;
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), &cam.getViewMat4()[0]);
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), &cam.getProjectionMat4()[0]);

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
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));
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
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

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

void GraphicTest::_blend(Window* window)
{
    window->enableZTest(true);
    window->enableStencil(false);
    //auto& blend = window->getBlend();
    //blend.SetActive(false);

    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.01f);

    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

    glm::vec3 planePos = glm::vec3(0,-0.5,0);
    glm::vec3 planeSize = glm::vec3(100,0.1,100);

    std::vector<glm::vec3> cubePoses = {
        glm::vec3(2,0,0),
        glm::vec3(-1,0,-1)
    };

    Texture* cubeTex = new Texture("Blend/cube.jpg", Diffuse);
    Material* cubeMaterial = new Material("Blend/blendTest", {cubeTex});

    Texture* planeTex = new Texture("Blend/plane.png", Diffuse);
    Material* planeMaterial = new Material("Blend/blendTest", {planeTex});
    glm::mat4 planeModel(1.0f);
    planeModel = glm::translate(planeModel, planePos);
    planeModel = glm::scale(planeModel, planeSize);
    planeMaterial->SetShaderParam("model", planeModel);

    Mesh* plane = new Mesh(Vertex::boxElement, Vertex::box, planeMaterial, "plane");

    Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMaterial, "cube");



    Texture* windowTex = new Texture("Blend/window.png", Diffuse);
    Material* windowMaterial = new Material("Blend/blendTest", {windowTex});
    std::vector<glm::vec3> windowPoses
    {
        glm::vec3(-1.5f, 0.0f, -0.48f * 2),
        glm::vec3( 1.5f, 0.0f, 0.51f * 2),
        glm::vec3( 0.0f, 0.0f, 0.7f * 2),
        glm::vec3(-0.3f, 0.0f, -2.3f * 2),
        glm::vec3( 0.5f, 0.0f, -0.6f * 2)
    };  
    Mesh* windowPlane = new Mesh(Vertex::quadElement, Vertex::quad, windowMaterial, "window");


    window->AddUpdateCallback([window, windowPlane, windowPoses, cube, cubePoses, plane](){
        GlobalShaderParam* gsp = GlobalShaderParam::Get();
        Camera& cam = window->getCamera();
        auto& st = window->getStencilTest();
        glm::mat4 view = cam.getViewMat4();
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));
        gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

        plane->draw();

        for (int i = 0; i < cubePoses.size(); i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePoses[i]);
            cube->SetShaderParam("model", model);
            cube->draw();
        }

        std::map<float, glm::vec3> sorted;


        for (int i = 0; i < windowPoses.size(); i++)
        {
            float dis = glm::length(cam.getCameraPos() - windowPoses[i]);
            sorted[dis] = windowPoses[i];
        }

        for (auto it = sorted.rbegin(); it != sorted.rend(); it++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, it->second);
            windowPlane->SetShaderParam("model", model);
            windowPlane->draw();
        }
    });
}

void GraphicTest::_faceCulling(Window* window)
{
    std::vector<Vertex> vertexes = {
        // Back face
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f},{},{},{}}, // Bottom-left
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f},{},{},{}}, // top-right
        {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f},{},{},{}}, // bottom-right         
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f},{},{},{}}, // top-right
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f},{},{},{}}, // bottom-left
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // top-left
        // Front face
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f},{},{},{}}, // bottom-left
        {{0.5f, -0.5f,  0.5f},  {1.0f, 0.0f},{},{},{}}, // bottom-right
        {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f},{},{},{}}, // top-right
        {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f},{},{},{}}, // top-right
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f},{},{},{}}, // top-left
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f},{},{},{}}, // bottom-left
        // Left face
        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f},{},{},{}}, // top-right
        {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f},{},{},{}}, // top-left
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // bottom-left
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // bottom-left
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f},{},{},{}}, // bottom-right
        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f},{},{},{}}, // top-right
        // Right face
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f},{},{},{}}, // top-left
        {{0.5f, -0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // bottom-right
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f},{},{},{}}, // top-right         
        {{0.5f, -0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // bottom-right
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f},{},{},{}}, // top-left
        {{0.5f, -0.5f,  0.5f},  {0.0f, 0.0f},{},{},{}}, // bottom-left     
        // Bottom face
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // top-right
        {{0.5f, -0.5f, -0.5f },  {1.0f, 1.0f},{},{},{}}, // top-left
        {{0.5f, -0.5f,  0.5f },  {1.0f, 0.0f},{},{},{}}, // bottom-left
        {{0.5f, -0.5f,  0.5f },  {1.0f, 0.0f},{},{},{}}, // bottom-left
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f},{},{},{}}, // bottom-right
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // top-right
        // Top face
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // top-left
        {{0.5f,  0.5f,  0.5f },  {1.0f, 0.0f},{},{},{}}, // bottom-right
        {{0.5f,  0.5f, -0.5f },  {1.0f, 1.0f},{},{},{}}, // top-right     
        {{0.5f,  0.5f,  0.5f },  {1.0f, 0.0f},{},{},{}}, // bottom-right
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f},{},{},{}}, // top-left
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f},{},{},{}} // bottom-left        
    };

    window->enableZTest(true);
    window->enableStencil(false);
    //auto& blend = window->getBlend();
    //blend.SetActive(false);

    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.01f);

    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

    Texture* cubeTex = new Texture("Blend/cube.jpg", Diffuse);
    Material* cubeMaterial = new Material("Blend/blendTest", {cubeTex});
    cubeMaterial->SetShaderParam("model", glm::mat4(1.0f));
    Mesh* cube = new Mesh({}, vertexes, cubeMaterial, "cube");

    auto& faceCulling = window->getFaceCulling();
    faceCulling.SetActive(true);
    faceCulling.SetCullingFace(Render::FaceCulling::Front);
    faceCulling.SetWindingOrder(Render::FaceCulling::CounterClockWise);

    window->AddUpdateCallback([cube, window](){
        Camera& cam = window->getCamera();
        GlobalShaderParam* gsp = GlobalShaderParam::Get();
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

        cube->draw();
    });

}

void GraphicTest::_frameBuffer(Window* window)
{
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);
    GlobalShaderParam* gsp = GlobalShaderParam::Get();
    gsp->GenBlock("GlobalMatrices", 2 * sizeof(glm::mat4), nullptr);
    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(cam.getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));

    gsp->GenBlock("GlobalPositions", 2 * sizeof(glm::vec4), nullptr);
    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

    glm::vec3 planePos = glm::vec3(0,-0.5,0);
    glm::vec3 planeSize = glm::vec3(100,0.1,100);

    std::vector<glm::vec3> cubePoses = {
        glm::vec3(2,0,0),
        glm::vec3(-1,0,-1)
    };

    Texture* cubeTex = new Texture("Blend/cube.jpg", Diffuse);
    Material* cubeMaterial = new Material("Blend/blendTest", {cubeTex});

    Texture* planeTex = new Texture("Blend/plane.png", Diffuse);
    Material* planeMaterial = new Material("Blend/blendTest", {planeTex});
    glm::mat4 planeModel(1.0f);
    planeModel = glm::translate(planeModel, planePos);
    planeModel = glm::scale(planeModel, planeSize);
    planeMaterial->SetShaderParam("model", planeModel);

    Mesh* plane = new Mesh(Vertex::boxElement, Vertex::box, planeMaterial, "plane");

    Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMaterial, "cube");

    auto& frameBuffer = window->getFrameBuffer();
    frameBuffer.SetActive(true);
    frameBuffer.AddTextureAttachment(Render::FrameBuffer::AttachmentType::Color0);
    //frameBuffer.AddTextureAttachment(Render::FrameBuffer::AttachmentType::Depth, Graphic::Texture::DepthComponent, Graphic::Texture::Depth);

    frameBuffer.AddRenderBuffer(Render::FrameBuffer::AttachmentType::Depth);

    //frameBuffer.SetShader("FrameBuffer/Depth");

    window->AddUpdateCallback([window, cube, plane, cubePoses](){
        GlobalShaderParam* gsp = GlobalShaderParam::Get();
        Camera& cam = window->getCamera();
        glm::mat4 view = cam.getViewMat4();
        gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(view));
        gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getProjectionMat4()));
        gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(cam.getCameraPos()));

        plane->draw();

        for (int i = 0; i < cubePoses.size(); i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePoses[i]);
            cube->SetShaderParam("model", model);
            cube->draw();
        }
    });

}

void GraphicTest::_renderQueue(Window* window)
{
    auto& renderQueue = window->getRenderQueue();
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);

    glm::vec3 planePos = glm::vec3(0,-0.5,0);
    glm::vec3 planeSize = glm::vec3(100,0.1,100);

    Texture* planeTex = new Texture("Blend/plane.png", Diffuse);
    Material* planeMaterial = new Material("Blend/blendTest", {planeTex});
    Mesh* plane = new Mesh(Vertex::boxElement, Vertex::box, planeMaterial, "plane");
    plane->SetPosition(planePos);
    plane->SetSize(planeSize);
    renderQueue.EnqueMesh(plane);

    std::vector<glm::vec3> cubePoses = {
        glm::vec3(0,2,0),
        glm::vec3(2,0.5,0),
        glm::vec3(-1,3,-1)
    };

    Texture* cubeTex = new Texture("Blend/cube.jpg", Diffuse);
    Material* cubeMaterial = new Material("Blend/blendTest", {cubeTex});
    Mesh* cube = new Mesh(Vertex::boxElement, Vertex::box, cubeMaterial, "cube");
    for (int i = 0; i < cubePoses.size(); i++)
    {
        cube->SetPosition(cubePoses[i]);
        renderQueue.EnqueMesh(cube);
        cube = cube->Clone();
    }
    
}

void GraphicTest::_shadowMapping(Window* window)
{
    auto& rq = window->getRenderQueue();
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);
    Texture* depthTexture = window->getShadowMapping().GetDepthTexture();

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


#include "system/RenderSystem.h"
void GraphicTest::_shadowMapping_engine()
{
    RenderSystem* rdSystem = RenderSystem::Get();
    auto& rq = rdSystem->getRenderQueue();
    Entity::Camera& cam = rdSystem->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.02f);
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