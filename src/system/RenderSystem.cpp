#include "RenderSystem.h"
#include "engine/Engine.h"
#include "WindowSystem.h"
void RenderSystem::Init()
{
    // todo: Camera created by EntitySystem
    m_mainCamera = new Entity::Camera();

    m_frameBuffer.Init();
    m_stencilTest.Init();
    m_depthTest.Init();
    m_blend.Init();
    m_faceCulling.Init();
    m_shadowMapping.Init();
    m_frameBuffer.SetSize(Engine::GetWindowWidth(), Engine::GetWindowHeight());
    m_shadowMapping.SetUp(Engine::GetWindowWidth(), Engine::GetWindowHeight(), &m_renderQueue);
}

void RenderSystem::Update()
{
    m_mainCamera->update();


    m_frameBuffer.Bind();
    glClearColor(0,0,0,1);
    GLbitfield mask = GL_COLOR_BUFFER_BIT;
    mask |= m_stencilTest.ClearMask();
    mask |= m_depthTest.ClearMask();
    glClear(mask);

    // 1st pass
    m_shadowMapping.DepthPass();
    
    // 2nd pass
    {
        m_shadowMapping.ShadowPass(m_frameBuffer.GetId());
        m_renderQueue.Render(Render::RenderQueue::Geometry);
        m_renderQueue.Render(Render::RenderQueue::Transparent);
    }
        
    //InvokePreUpdateCallback();
    //InvokeUpdateCallback();
    //InvokePostUpdateCallback();

    //3rd pass
    if (m_frameBuffer.IsValid())
    {
        m_frameBuffer.UnBind();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_frameBuffer.Draw();
    }
    
    
    
    m_renderQueue.Render(Render::RenderQueue::Overlay);
    glfwSwapBuffers(WindowSystem::Get()->getGLFWwindow());
    glfwPollEvents();
}