#ifndef _WINDOW_H__
#define _WINDOW_H__

#include "common/headers.h"
#include <functional>
#include <vector>
#include <map>
#include "basicTypes.h"
#include "pipline/Camera.h"
namespace Pipline
{
    class Window;
    class Camera;
    class WindowManager
    {
    DECLARE_SINGLETON(WindowManager)
    private:
        std::vector<Window*> m_windows;
    public:
        Window* GenerateWindow(int width, int height, const char* title);
        Window* GetWindow(int id);
        void DeleteWindow(int id);
        inline int GetWindowNum() const { return m_windows.size(); }
        
    public:
        void InvokeWindowResizeCallback(GLFWwindow* window, int width, int height);
    };



    class Window
    {
    friend WindowManager;
    private:
        GLFWwindow* m_window = nullptr;
        int m_width;
        int m_height;
        int m_id;
        std::string m_title;
        Color m_bgColor;
        std::vector<std::function<void(int,int)>> m_resizeCallbacks;
        std::vector<std::function<void()> > m_updateCallbacks;
        std::vector<std::function<void()> > m_preUpdateCallbacks;
        std::vector<std::function<void()> > m_postUpdateCallbacks;
    private:
        bool m_enableZTest = false;
    private:
        Camera m_camera;
    private:
        void init();
        void InvokeResizeCallbacks(GLFWwindow* window, int width, int height);
        void InvokeUpdateCallback();
        void InvokePreUpdateCallback();
        void InvokePostUpdateCallback();
    public:
        Window(int width, int height, const char* title);
        ~Window() { delete m_window; m_window = nullptr; }
    
    public:
        void doUpdate();
        void Close();

        void SetBgColor(const Color& color) { m_bgColor = color; }

        inline int getWindowHeight() const { return m_height; }
        inline int getWindowWidth() const { return m_width; }
        inline std::string getTitle() const { return m_title; }
        inline int getWindowID() const { return m_id; }
        inline GLFWwindow* getGLFWwindow() const { return m_window; }

        inline void enableZTest(bool v) { m_enableZTest = v; }
        inline bool isEnableZTest() const { return m_enableZTest;}
    public:
        int AddFramebufferSizeCallback(std::function<void(int,int)> func);
        void DeleteFramebufferSizeCallback(int id);

        int AddUpdateCallback(std::function<void()> func);
        void DeleteUpdateCallback(int id);

        int AddPreUpdateCallback(std::function<void()> func);
        void DeletePreUpdateCallback(int id);

        int AddPostUpdateCallback(std::function<void()> func);
        void DeletePostUpdateCallback(int id);
    public:
        Camera& getCamera() { return m_camera; }
    };
}

#endif