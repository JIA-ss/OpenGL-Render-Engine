#include "pipline/window.h"
using namespace Pipline;
int main()
{
    //return cameraMain();
    //return HELLOTRIANGLE::helloTriangleRenderLoop();
    //return Review::reviewMain();

    Window* window = WindowManager::Instance()->GenerateWindow(800, 600, "test");
    window->AddFramebufferSizeCallback([window](int width, int height)
    {
        std::cout << "Window Resize:" << width << ", " << height << std::endl;
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window->getGLFWwindow());
    });
    window->SetBgColor(Color(0.5,0.5,0.5,0.5));

    window->AddUpdateCallback([]()
    {
        std::cout << "update" << std::endl;
    });

    window->doUpdate();
    return 1;
}