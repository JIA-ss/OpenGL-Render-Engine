#include "RenderSystem.h"
#include "engine/Engine.h"
#include "WindowSystem.h"
#include "render/RenderPath/RenderPathBase.h"
#include <TracyOpenGL.hpp>
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
    m_mainCamera->update();

    m_deferred.RenderPasses();
    m_forward.RenderPasses();

    glfwSwapBuffers(WindowSystem::Get()->getGLFWwindow());
    //TracyGpuContext
    glfwPollEvents();
}