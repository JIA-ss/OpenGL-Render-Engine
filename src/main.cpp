#include "pipline/window.h"
#include "pipline/Shader.h"
#include "pipline/bufferUtil.h"
#include "tools/fileWatcher.h"
#include "resource/resourceManager.h"
#include <string>
#include "test/test.h"
#include "tools/ioUtil.h"
#include "input/InputManager.h"
#include "test/graphicTest.h"
using namespace Pipline;

void InitSingleTons()
{
    // Init SingleTon
    WindowManager::InitSingleTon();
    Util::FileWatcherManager::InitSingleTon();
    Resource::ResourceManager::InitSingleTon();
}

void InitMgrs()
{
    // Init Mgr
    Resource::ResourceManager::Instance()->Init();
}

Window* WindowInit()
{
    // Init Window
    Window* window = WindowManager::Instance()->GenerateWindow(800, 600, "test");
    window->AddPreUpdateCallback([](){ Util::FileWatcherManager::Instance()->Update(); });


    window->AddFramebufferSizeCallback([window](int width, int height)
    {
        std::cout << "Window Resize:" << width << ", " << height << std::endl;
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window->getGLFWwindow());
    });

    window->SetBgColor(Color{0.5,0.5,0.5,0.5});
    WindowManager::Instance()->SetValidWindow(window);
    return window;
}


int main()
{
    InitSingleTons();
    InitMgrs();
    Window* window = WindowInit();


    //test::drawTriangle(window);
    //test::drawTextureWithBuff(window);
    //test::drawTwoTexture(window);
    //test::drawTwoTextureWithBuff(window);
    //test::transformTest(window);
    //test::_3Dtest(window);
    //test::_3DMultiSmile(window);
    //test::_3DtestMuiltiCube(window);

    //test::camera(window);
    //test::cameraControl(window);

    //test::color(window);
    //test::reflect(window);

    //test::AssimpLinkTest(window);
    std::cout << "main start" << std::endl;
    Util::testJson();
    //test::ModelTest(window);
    //test::transformModify(window);
    
    //test::atlasTest(window);
    GraphicTest::simpleImage(window);
    window->doUpdate();
    return 1;
}