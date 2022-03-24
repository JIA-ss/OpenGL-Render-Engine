#include "PostProcessing.h"
#include "engine/Engine.h"

RENDER_NAMESPACE_USING

void PostProcessing::Init()
{
    if (m_enable)
    {
        m_buffer.SetActive(true);
        m_buffer.SetSize(Engine::GetWindowWidth(), Engine::GetWindowHeight());
        m_buffer.AddTextureAttachment(FrameBuffer::Color0);
        m_buffer.AddRenderBuffer(FrameBuffer::DepthStencil);
        m_buffer.SetShader("FrameBuffer/Default");
        m_buffer.Init();
    }
    else
    {
        m_buffer.SetActive(false);
        m_buffer.Init();
    }
}

void PostProcessing::SetUp() const 
{
    if (!m_enable)
        return;
}


void PostProcessing::PrepareRenderToTexture()
{
    m_buffer.Bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);
    GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    glClear(mask);
}

void PostProcessing::TextureToScreen()
{
    m_buffer.UnBind();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_buffer.Draw();
}

GLuint PostProcessing::GetBufferId() const
{
    return m_buffer.GetId();
}
