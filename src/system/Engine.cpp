#include "Engine.h"
#include "WindowSystem.h"
#include "InputSystem.h"
#include "EntitySystem.h"
#include "RenderSystem.h"

#include "tools/fileWatcher.h"
#include "resource/resourceManager.h"
Engine* Engine::_instance_ = nullptr;
SystemManager* Engine::_system_manager_ = nullptr;
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
    
    {
        // todo: refactor later
        Util::FileWatcherManager::InitSingleTon();
        Resource::ResourceManager::InitSingleTon();
        Resource::ResourceManager::Instance()->Init();
    }

    Engine* engine = Engine::Instance();

    _system_manager_ = SystemManager::Instance();
    
    engine->m_windowSystem = _system_manager_->AddSystem<WindowSystem>();
    engine->m_windowSystem->setWindowInfo(_window_width_, _window_height_, _window_title_);

    _system_manager_->AddSystem<InputSystem>();
    _system_manager_->AddSystem<EntitySystem>();
    _system_manager_->AddSystem<RenderSystem>();



    SystemManager::Instance()->Init();
    return engine;
}

void Engine::StartEngine()
{
    while(!glfwWindowShouldClose(m_windowSystem->getGLFWwindow()))
    {
        _system_manager_->Update();
    }

    glfwTerminate();
}

void Engine::StopEngine()
{
    glfwSetWindowShouldClose(m_windowSystem->getGLFWwindow(), true);
}