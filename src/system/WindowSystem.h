#pragma once
#include "System.h"
#include "common/headers.h"

#include <functional>


class WindowSystem: public System
{
public:
    SYSTEM_STATIC_TYPE(WindowSystem, System::Type::WindowSystem)
public:
    WindowSystem() : System(System::WindowSystem) { }
    ~WindowSystem();
    void Init() override;
    void Update() override;

private:
    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;
    std::string m_title;

    std::vector<std::function<void(int,int)>> m_resizeCallbacks;
    std::vector<std::function<void()> > m_updateCallbacks;
    std::vector<std::function<void()> > m_preUpdateCallbacks;
    std::vector<std::function<void()> > m_postUpdateCallbacks;
public:
    int AddWindowResizeCallback(std::function<void(int,int)> func);
    void DeleteWindowResizeCallback(int id);

public:
    inline int getWindowHeight() const { return m_height; }
    inline int getWindowWidth() const { return m_width; }
    inline std::string getTitle() const { return m_title; }
    inline GLFWwindow* getGLFWwindow() const { return m_window; }

    void setWindowInfo(int width, int height, const std::string& title);
public:
    void InvokeResizeCallbacks(GLFWwindow* window, int width, int height);
};