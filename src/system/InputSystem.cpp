#include "InputSystem.h"

void InputSystem::Init()
{
    m_windowSystem = SystemManager::Instance()->GetSystem<::WindowSystem>();
    glfwSetScrollCallback(m_windowSystem->getGLFWwindow(), [](GLFWwindow* window, double v1, double v2){
        SystemManager::Instance()->GetSystem<::InputSystem>()->scroll_callback(window,v1,v2);
    });
}

void InputSystem::Update()
{
    double xPos, yPos;
    glfwGetCursorPos(m_windowSystem->getGLFWwindow(), &xPos, &yPos);
    m_lastFrameMousePos = m_curFrameMousePos;
    m_curFrameMousePos = glm::vec2(xPos, yPos);

    //std::cout << "lastPos: " << m_lastFrameMousePos.x  << '\t' << m_lastFrameMousePos.y << std::endl;
    //std::cout << "curPos: " << m_curFrameMousePos.x  << '\t' << m_curFrameMousePos.y << std::endl;

    if (m_framNumTmp > 1)
    {
        m_scrollValue = glm::vec2(0.0f);
        m_framNumTmp = 0;
    }

    //std::cout << "scroll: " << m_scrollValue.x << '\t' << m_scrollValue.y << std::endl;
    m_framNumTmp++;
}


void InputSystem::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (window != m_windowSystem->getGLFWwindow())
        return;
    m_framNumTmp = 0;
    m_scrollValue = glm::vec2(xoffset, yoffset);
}

int InputSystem::GetKeyState(int key)
{
    return glfwGetKey(m_windowSystem->getGLFWwindow(), key);
}

bool InputSystem::IsKeyPressed(int key)
{
    return GetKeyState(key) == GLFW_PRESS;
}

bool InputSystem::IsKeyReleased(int key)
{
    return GetKeyState(key) == GLFW_RELEASE;
}