#include "window.h"
#include "input/InputManager.h"
using namespace Pipline;

IMPLEMENT_SINGLETON(WindowManager)

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


void WindowManager::RecordScrollInfo(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
    for (Window* window : m_windows)
    {
        window->getInputMgr()->scroll_callback(glfwWindow, xOffset, yOffset);
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
    m_bgColor = Color{0,0,0,0};
    m_camera.setWindow(this);
    init();
}

void Window::init()
{
    m_inputMgr = new InputManager();
    m_inputMgr->setWindow(this);

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

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double v1, double v2){
        WindowManager::Instance()->RecordScrollInfo(window, v1, v2);
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
    m_resizeCallbacks.push_back(func);
    return id;
}

void Window::DeleteFramebufferSizeCallback(int id)
{
    if (id >= 0 && id < m_resizeCallbacks.size())
    {
        m_resizeCallbacks.erase(m_resizeCallbacks.begin() + id);
    }
}

int Window::AddUpdateCallback(std::function<void()> func)
{ 
    m_updateCallbacks.push_back(func); 
    return m_updateCallbacks.size() - 1; 
}

void Window::DeleteUpdateCallback(int id)
{
    if (id >= 0 && id < m_updateCallbacks.size())
    {
        m_updateCallbacks.erase(m_updateCallbacks.begin() + id);
    }
}

void Window::InvokeUpdateCallback()
{
    for (auto&& func : m_updateCallbacks)
    {
        func();
    }
}

int Window::AddPreUpdateCallback(std::function<void()> func)
{ 
    m_preUpdateCallbacks.push_back(func); 
    return m_preUpdateCallbacks.size() - 1; 
}

void Window::DeletePreUpdateCallback(int id)
{
    if (id >= 0 && id < m_preUpdateCallbacks.size())
    {
        m_preUpdateCallbacks.erase(m_preUpdateCallbacks.begin() + id);
    }
}

void Window::InvokePreUpdateCallback()
{
    for (auto&& func : m_preUpdateCallbacks)
    {
        func();
    }
}

    int Window::AddPostUpdateCallback(std::function<void()> func)
{ 
    m_postUpdateCallbacks.push_back(func); 
    return m_postUpdateCallbacks.size() - 1; 
}

void Window::DeletePostUpdateCallback(int id)
{
    if (id >= 0 && id < m_postUpdateCallbacks.size())
    {
        m_postUpdateCallbacks.erase(m_postUpdateCallbacks.begin() + id);
    }
}

void Window::InvokePostUpdateCallback()
{
    for (auto&& func : m_postUpdateCallbacks)
    {
        func();
    }
}


void Window::InvokeResizeCallbacks(GLFWwindow* window, int width, int height)
{
    if (window != m_window)
        return;

    for(auto&& func : m_resizeCallbacks)
    {
        func(width, height);
    }

    m_width = width;
    m_height = height;
}


void Window::doUpdate()
{
    if (m_enableZTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(m_window))
    {
        glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
        GLbitfield mask = GL_COLOR_BUFFER_BIT;
        if (m_enableZTest)
            mask |= GL_DEPTH_BUFFER_BIT;
        glClear(mask);
        
        m_inputMgr->update();
        m_camera.processControl();

        InvokePreUpdateCallback();
        InvokeUpdateCallback();
        InvokePostUpdateCallback();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_window, true);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (window != getGLFWwindow())
        return;
    static bool firstFrame = true;
    Vector2 scrollValue{xoffset, yoffset};
    if (firstFrame)
    {
        firstFrame = false;
        m_lastFrameScrollValue = scrollValue;
        m_curFrameScrollValue = scrollValue;
        return;
    }

    m_lastFrameScrollValue = m_curFrameScrollValue;
    m_curFrameScrollValue = scrollValue;
}