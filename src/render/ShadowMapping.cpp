#include "ShadowMapping.h"

RENDER_NAMESPACE_USING

void ShadowMapping::Init()
{
    if (m_enable)
    {
        InitDepthMap();
    }
    else
    {
        return;
    }
}

void ShadowMapping::InitDepthMap()
{
    m_depthMapFrameBuffer.SetActive(true);
    m_depthMapFrameBuffer.SetSize(m_shadowPrecision, m_shadowPrecision);

    m_depthMapFrameBuffer.AddTextureAttachment
    (
        FrameBuffer::Depth, 
        Graphic::Texture::DepthComponent, 
        Graphic::Texture::Depth, 
        Graphic::Texture::Float
    );

    m_depthMapFrameBuffer.Init();
}

void ShadowMapping::SetSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void ShadowMapping::SetLightView(const glm::vec3& from, const glm::vec3& to)
{
    m_lightView = glm::lookAt(from, to, glm::vec3(0,1,0));
}

void ShadowMapping::SetLightProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near , GLfloat far)
{
    if (near == 0)
    {
        m_lightProjection = glm::ortho(left, right, bottom, top);
    }
    else
    {
        m_lightProjection = glm::ortho(left,right,bottom,top,near,far);
    }
}

glm::mat4 ShadowMapping::GetLightSpaceMatrice() const
{
    return m_lightProjection * m_lightView;
}

void ShadowMapping::DepthPass()
{
    glViewport(0, 0, m_shadowPrecision, m_shadowPrecision);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFrameBuffer.GetId());
    glClear(GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void ShadowMapping::LightPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}