#ifndef _WINDOW_H__
#define _WINDOW_H__

#include "common/headers.h"
#include <functional>
#include <vector>
#include <map>
#include "basicTypes.h"
#include "pipline/Camera.h"
#include "render/DepthTest.h"
#include "render/StencilTest.h"
namespace Pipline
{
    class Window;
    class Camera;
    class WindowManager
    {
    DECLARE_SINGLETON(WindowManager)
    private:
        std::vector<Window*> m_windows;
        Window* m_curValidWindow = nullptr;
    public:
        Window* GenerateWindow(int width, int height, const char* title);
        Window* GetWindow(int id);
        void DeleteWindow(int id);
        inline int GetWindowNum() const { return m_windows.size(); }
        Window* GetCurWindow() const { return m_curValidWindow; }
        inline void SetValidWindow(Window* window) { m_curValidWindow = window; }
    public:
        void InvokeWindowResizeCallback(GLFWwindow* window, int width, int height);
        void RecordScrollInfo(GLFWwindow* window, double xOffset, double yOffset);
    };


    class InputManager;
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
        Vector2 m_lastFrameMousePos;
        Vector2 m_curFrameMousePos;
        Vector2 m_lastFrameScrollValue;
        Vector2 m_curFrameScrollValue;
        std::vector<std::function<void(int,int)>> m_resizeCallbacks;
        std::vector<std::function<void()> > m_updateCallbacks;
        std::vector<std::function<void()> > m_preUpdateCallbacks;
        std::vector<std::function<void()> > m_postUpdateCallbacks;
        InputManager* m_inputMgr;
    private:
        Render::DepthTest m_depthTest;
        Render::StencilTest m_stencilTest;
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

        inline void enableZTest(bool v) { m_depthTest.SetActive(v); }
        inline bool isEnableZTest() const { return m_depthTest.isActive(); }
        inline void enableStencil(bool v) { m_stencilTest.SetActive(v); }
        inline bool isEnableStencil() const { return m_stencilTest.isActive(); }
        inline void setDepthTestFunc(Render::DepthTest::DepthFunc func) { m_depthTest.SetFunc(func); }
        inline Render::StencilTest& getStencilTest() { return m_stencilTest; }
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
        inline Camera& getCamera() { return m_camera; }
        InputManager* getInputMgr() { return m_inputMgr; };
        inline Vector2 getCurMousePos() const { return m_curFrameMousePos; }
        inline Vector2 getPreMousePos() const { return m_lastFrameMousePos; }
        inline Vector2 getCurScrollValue() const { return m_curFrameScrollValue; }
        inline Vector2 getPreScrollValue() const { return m_lastFrameScrollValue; }
    private:
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    };
};

#endif