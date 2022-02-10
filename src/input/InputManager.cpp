#include "InputManager.h"
#include "pipline/window.h"

using namespace Pipline;

void InputManager::Init()
{

}

void InputManager::update()
{
    double xPos, yPos;
    glfwGetCursorPos(m_parentWindow->getGLFWwindow(), &xPos, &yPos);
    m_lastFrameMousePos = m_curFrameMousePos;
    m_curFrameMousePos = Vector2{(float)xPos, (float)yPos};

    //std::cout << "lastPos: " << m_lastFrameMousePos.x  << '\t' << m_lastFrameMousePos.y << std::endl;
    //std::cout << "curPos: " << m_curFrameMousePos.x  << '\t' << m_curFrameMousePos.y << std::endl;

    if (m_framNumTmp > 1)
    {
        m_scrollValue = Vector2{0,0};
        m_framNumTmp = 0;
    }

    //std::cout << "scroll: " << m_scrollValue.x << '\t' << m_scrollValue.y << std::endl;
    m_framNumTmp++;
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (window != m_parentWindow->getGLFWwindow())
        return;
    m_framNumTmp = 0;
    m_scrollValue = Vector2{(float)xoffset, (float)yoffset};
}
