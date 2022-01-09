#include "pipline/window.h"

using namespace Pipline;
int main()
{
    WindowManager::InitSingleTon();
    Window* window = WindowManager::Instance()->GenerateWindow(800, 600, "test");
    window->AddFramebufferSizeCallback([window](int width, int height)
    {
        std::cout << "Window Resize:" << width << ", " << height << std::endl;
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window->getGLFWwindow());
    });
    window->SetBgColor(Color(0.5,0.5,0.5,0.5));
    
    window->AddPreUpdateCallback([]()
    {
        std::vector<float> vertices = 
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    });

    window->AddUpdateCallback([]()
    {

    });

    window->AddPostUpdateCallback([](){

    });

    window->doUpdate();
    return 1;
}