#ifndef _WINDOW_H__
#define _WINDOW_H__

#include "common/headers.h"
#include <functional>
#include <vector>
#include <map>
#include "basicTypes.h"
namespace Pipline
{
    class Window;
    class WindowManager
    {
    private:
        std::vector<Window*> m_windows;
        static WindowManager* _instance_;
    public:
        static WindowManager* Instance()
        {
            if (!_instance_)
            {
                _instance_ = new WindowManager();
            }
            return _instance_;
        }

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
    private:
        void init();
        void InvokeResizeCallbacks(GLFWwindow* window, int width, int height);
        void InvokeUpdateCallback();
    public:
        Window(int width, int height, const char* title);
        ~Window();
    
    public:
        void doUpdate();
        int AddFramebufferSizeCallback(std::function<void(int,int)> func);
        void DeleteFramebufferSizeCallback(int id);

        int AddUpdateCallback(std::function<void()> func);
        void DeleteUpdateCallback(int id);

        inline void CloseWindow();
        void SetBgColor(const Color& color) { m_bgColor = color; }

        inline int getWindowHeight() const { return m_height; }
        inline int getWindowWidth() const { return m_width; }
        inline std::string getTitle() const { return m_title; }
        inline int getWindowID() const { return m_id; }
        inline GLFWwindow* getGLFWwindow() const { return m_window; }
    };
}

#endif