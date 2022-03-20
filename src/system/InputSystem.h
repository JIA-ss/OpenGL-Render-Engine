#pragma once
#include "System.h"
#include "WindowSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class InputSystem : public System
{
    SYSTEM_STATIC_TYPE(InputSystem, System::Type::InputSystem)
public:
    InputSystem() : System(System::Type::InputSystem) { }
private:
    glm::vec2 m_lastFrameMousePos;
    glm::vec2 m_curFrameMousePos;
    glm::vec2 m_scrollValue;
    unsigned int m_framNumTmp = 0;

    ::WindowSystem* m_windowSystem = nullptr;
public:
    void Init() override;
    void Update() override;
public:
    int GetKeyState(int key);
    bool IsKeyPressed(int key);
    bool IsKeyReleased(int key);

    inline glm::vec2 getCurMousePos() const { return m_curFrameMousePos; }
    inline glm::vec2 getPreMousePos() const { return m_lastFrameMousePos; }
    inline glm::vec2 getScrollValue() const { return m_scrollValue; }
public:
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};