#pragma once
#include "common/headers.h"
#include <functional>
#include <vector>
#include <map>
#include "pipline/basicTypes.h"
#include "pipline/window.h"
namespace Pipline
{
class InputManager
{

private:
    Pipline::Window* m_parentWindow = nullptr;

    Vector2 m_lastFrameMousePos;
    Vector2 m_curFrameMousePos;

    Vector2 m_scrollValue;

    unsigned int m_framNumTmp = 0;
public:
    void Init();
    void update();
    void setWindow(Pipline::Window* w) { m_parentWindow = w; }
public:
    inline Vector2 getCurMousePos() const { return m_curFrameMousePos; }
    inline Vector2 getPreMousePos() const { return m_lastFrameMousePos; }
    inline Vector2 getScrollValue() const { return m_scrollValue; }
public:
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
}