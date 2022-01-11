#include "pipline/window.h"
#include "pipline/Shader.h"
#include "pipline/bufferUtil.h"
#include "tools/fileWatcher.h"
#include "resource/resourceManager.h"
#include <string>
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
    window->SetBgColor(Color(0.5,0.5,0.5,0.5));

    return window;
}


int main()
{
    InitSingleTons();
    InitMgrs();
    Window* window = WindowInit();

    float vertex[] = 
    {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    unsigned int indices[] = 
    {
        0,1,2,
        1,2,3,
        0,2,3,
        0,1,3
    };
    Pipline::Shader shader = Pipline::Shader("shader1.vs", "shader2.fs");
    unsigned int vao = Pipline::GenerateVao(vertex, sizeof(vertex), indices, sizeof(indices));

    window->AddPreUpdateCallback([&shader, &vao]()
    {
        shader.use();
        static float colorValue = 0.0f;
        float timeValue = glfwGetTime();
        shader.setFloat("xOffSet", (colorValue + sin(timeValue) / 2.0f + 0.5f)/5.f);
        
        colorValue = sin(timeValue) / 2.0f + 0.5f;
        shader.setVec4("myColor", 0.0f,colorValue, 0.0f, 1.0f);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    });



    window->doUpdate();
    return 1;
}