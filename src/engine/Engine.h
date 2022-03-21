#pragma once
#include "common/headers.h"
#include "system/System.h"
#include "system/WindowSystem.h"

#include <vector>
#include <functional>

#define DEFAULT_WINDOW_WIDTH    1600
#define DEFAULT_WINDOW_HEIGHT   1024
#define DEFAULT_WINDOW_TITLE    "JoshuaEngine"

class Engine
{
public:
    static Engine*  Instance();
    static void     SetWindowInfo(int width, int height, const std::string& title);
    static Engine*  InitEngine();

    static int      GetWindowWidth() { return _window_width_; }
    static int      GetWindowHeight() { return _window_height_; }
public:
    void StartEngine();
    void StopEngine();

private:
    static Engine*          _instance_;
    static SystemManager*   _system_manager_;
    static WindowSystem*    _window_system_;
    static int              _window_width_;
    static int              _window_height_;
    static std::string      _window_title_;

};