#ifndef _WINDOW_H__
#define _WINDOW_H__

#include "common/headers.h"
namespace Pipline
{
    GLFWwindow* InitWindow(int width, int height, const char* title)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            // for MacOS
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "review window", NULL, NULL);
        if (window == nullptr)
        {
            std::cout << "create failed" << std::endl;
            glfwTerminate();
            return window;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            std::cout << "window resize width:" << width << ", height:" << height << std::endl;
        });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glViewport(0, 0, width, height);
            return window;
        }

        return window;
    }

    void doUpdate(GLFWwindow* window)
    {
        if (!window)
        {
            return;
        }

        while(!glfwWindowShouldClose(window))
        {

        }
    }
}

#endif