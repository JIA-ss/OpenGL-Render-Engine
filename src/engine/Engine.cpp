#include "Engine.h"
#include "system/WindowSystem.h"
#include "system/InputSystem.h"
#include "system/EntitySystem.h"
#include "system/RenderSystem.h"
#include "system/FileWatcherSystem.h"
#include "system/ResourceSystem.h"


Engine* Engine::_instance_ = nullptr;
SystemManager* Engine::_system_manager_ = nullptr;
WindowSystem* Engine::_window_system_ = nullptr;

int Engine::_window_width_ = DEFAULT_WINDOW_WIDTH;
int Engine::_window_height_ = DEFAULT_WINDOW_HEIGHT;
std::string Engine::_window_title_ = DEFAULT_WINDOW_TITLE;

Engine* Engine::Instance()
{
    if (_instance_ == nullptr)
        _instance_ = new Engine();
    return _instance_;
}

void Engine::SetWindowInfo(int width, int height, const std::string& title)
{
    _window_width_ = width;
    _window_height_ = height;
    _window_title_ = title;
}

Engine* Engine::InitEngine()
{
    SystemManager::InitSingleTon();
    Engine* engine = Engine::Instance();

    _system_manager_ = SystemManager::Instance();
    
    _window_system_ = _system_manager_->AddSystem<WindowSystem>();
    _window_system_->setWindowInfo(_window_width_, _window_height_, _window_title_);

    _system_manager_->AddSystem<InputSystem>();
    _system_manager_->AddSystem<EntitySystem>();
    _system_manager_->AddSystem<RenderSystem>();
    _system_manager_->AddSystem<FileWatcherSystem>();
    _system_manager_->AddSystem<ResourceSystem>();



    SystemManager::Instance()->Init();
    return engine;
}

void Engine::StartEngine()
{
    while(!glfwWindowShouldClose(_window_system_->getGLFWwindow()))
    {
        _system_manager_->Update();
    }

    glfwTerminate();

    _system_manager_->UnInit();
}

void Engine::StopEngine()
{
    glfwSetWindowShouldClose(_window_system_->getGLFWwindow(), true);
}