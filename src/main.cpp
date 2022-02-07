#include "pipline/window.h"
#include "pipline/Shader.h"
#include "pipline/bufferUtil.h"
#include "tools/fileWatcher.h"
#include "resource/resourceManager.h"
#include <string>
#include "test/test.h"
#include "tools/ioUtil.h"

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

    test::camera(window);
    window->doUpdate();
    return 1;
}