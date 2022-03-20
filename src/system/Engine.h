#pragma once
#include "common/headers.h"
#include "System.h"
#include "WindowSystem.h"

#include <vector>
#include <functional>

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080
#define DEFAULT_WINDOW_TITLE "JoshuaEngine"

class Engine
{
private:
    static Engine* _instance_;
    static SystemManager* _system_manager_;
    static int _window_width_;
    static int _window_height_;
    static std::string _window_title_;
public:
    static Engine* Instance();
    static void SetWindowInfo(int width, int height, const std::string& title);
    static Engine* InitEngine();

    static int GetWindowWidth() { return _window_width_; }
    static int GetWindowHeight() { return _window_height_; }
public:
    void StartEngine();
    void StopEngine();
private:
    WindowSystem* m_windowSystem = nullptr;
};