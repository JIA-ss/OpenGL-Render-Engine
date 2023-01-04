#include "RenderSystem.h"
#include "engine/Engine.h"
#include "WindowSystem.h"
#include "render/RenderPath/RenderPathBase.h"
#include <TracyOpenGL.hpp>
#include <chrono>
void RenderSystem::Init()
{
    ZoneScopedN("RenderSystem::Init");
    // todo: Camera created by EntitySystem
    m_mainCamera = new Entity::Camera();
    SetRenderPath(Render::RenderPath::Forward);
}

void RenderSystem::SetRenderPath(Render::RenderPath path)
{
    ZoneScopedN("RenderSystem::SetRenderPath");
    m_forward.Init(path);
    m_deferred.Init(path);
    m_renderPath = path;
}

Render::RenderPath RenderSystem::GetCurRenderPath() const
{
    return m_renderPath;
}


void RenderSystem::Update()
{
    ZoneScopedN("RenderSystem::Update");
    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    m_mainCamera->update();
    m_deferred.RenderPasses();
    m_forward.RenderPasses();

    {
        ZoneScopedN("RenderSystem::Update SwapWindowBuffers");
        glfwSwapBuffers(WindowSystem::Get()->getGLFWwindow());
    }
    {
        ZoneScopedN("RenderSystem::Update pollEvents");
        //TracyGpuContext
        glfwPollEvents();
    }

    auto end = clock.now();

    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    m_frame_time_ms += time_diff;
    m_frame_num++;
    if (m_frame_time_ms >= std::chrono::milliseconds{1000})
    {
        std::cout << "FrameRate: "<< m_frame_num << std::endl;
        m_frame_num = 0;
        m_frame_time_ms = std::chrono::milliseconds{0};
    }
}