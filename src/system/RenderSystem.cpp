#include "RenderSystem.h"
#include "engine/Engine.h"
#include "WindowSystem.h"
void RenderSystem::Init()
{
    // todo: Camera created by EntitySystem
    m_mainCamera = new Entity::Camera();
    //SetRenderPath(m_renderPath);
}

void RenderSystem::SetRenderPath(Render::RenderPath path)
{
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
    m_mainCamera->update();

    m_deferred.RenderPasses();
    m_forward.RenderPasses();

    glfwSwapBuffers(WindowSystem::Get()->getGLFWwindow());
    glfwPollEvents();
}