#include "WindowSystem.h"

WindowSystem::~WindowSystem()
{
    glfwTerminate();
    if (m_window)
    {
        delete m_window;
        m_window = nullptr;
    }
}

int WindowSystem::AddWindowResizeCallback(std::function<void(int,int)> func)
{
    int id = m_resizeCallbacks.size();
    m_resizeCallbacks.push_back(func);
    return id;
}

void WindowSystem::DeleteWindowResizeCallback(int id)
{
    if (id >= 0 && id < m_resizeCallbacks.size())
    {
        m_resizeCallbacks.erase(m_resizeCallbacks.begin() + id);
    }
}


void WindowSystem::InvokeResizeCallbacks(GLFWwindow* window, int width, int height)
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

void WindowSystem::setWindowInfo(int width, int height, const std::string& title)
{
    m_width = width;
    m_height = height;
    m_title = title;
}

void WindowSystem::Init()
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
        //System* system = SystemManager::Instance()->GetSystem(System::WindowSystem);
        //WindowSystem* w_system = static_cast<WindowSystem*>(system);
        WindowSystem* w_system = SystemManager::Instance()->GetSystem<WindowSystem>();
        if (w_system)
        {
            w_system->InvokeResizeCallbacks(window, width, height);
        }
        else
        {
            std::cout << "System* convert to WindowSystem* Failed!!" << std::endl;
        }
        glViewport(0,0,width, height);
    });

    //glfwSetScrollCallback(m_window, [](GLFWwindow* window, double v1, double v2){
    //    WindowManager::Instance()->RecordScrollInfo(window, v1, v2);
    //});

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glViewport(0, 0, m_width, m_height);
        return;
    }
}

void WindowSystem::Update()
{

}