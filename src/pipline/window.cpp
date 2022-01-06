#include "window.h"

using namespace Pipline;

    WindowManager* WindowManager::_instance_ = nullptr;
    
    Window* WindowManager::GenerateWindow(int width, int height, const char* title)
    {
        Window* window = new Window(width, height, title);
        int id = m_windows.size();
        window->m_id = id;
        m_windows.push_back(window);
        return window;
    }
    
    void WindowManager::InvokeWindowResizeCallback(GLFWwindow* window, int width, int height)
    {
        for (Window* _window : m_windows)
        {
            _window->InvokeResizeCallbacks(window,width,height);
        }
    }

    Window* WindowManager::GetWindow(int id) 
    {
        if (id < m_windows.size() && id >= 0)
        {
            return m_windows[id];
        }

        return nullptr;
    }

    void WindowManager::DeleteWindow(int id)
    {
        if (id < m_windows.size() && id >= 0)
        {
            m_windows.erase(m_windows.begin() + id);
        }
    }


    Window::Window(int width, int height, const char* title)
    {
        m_width = width;
        m_height = height;
        m_title = title;
        m_bgColor = Color(0,0,0,0);
        init();
    }

    void Window::init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            // for MacOS
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
        if (m_window == nullptr)
        {
            std::cout << "create failed" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_window);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            WindowManager::Instance()->InvokeWindowResizeCallback(window, width, height);
            glViewport(0,0,width, height);
        });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glViewport(0, 0, m_width, m_height);
            return;
        }
    }

    int Window::AddFramebufferSizeCallback(std::function<void(int,int)> func)
    {
        int id = m_resizeCallbacks.size();
        m_resizeCallbacks[id] = func;
        return id;
    }

    void Window::InvokeResizeCallbacks(GLFWwindow* window, int width, int height)
    {
        if (window != m_window)
            return;

        for(auto&& it : m_resizeCallbacks)
        {
            it.second(width, height);
        }

        m_width = width;
        m_height = height;
    }

    void Window::doUpdate()
    {
        while(!glfwWindowShouldClose(m_window))
        {
            glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

    void Window::CloseWindow()
    {
        glfwSetWindowShouldClose(m_window, true);
    }