#include "ShadowMapping.h"
#include "graphic/GlobalShaderParam.h"
RENDER_NAMESPACE_USING

void ShadowMapping::Init()
{
    if (m_enable)
    {
        InitLightMatrice();
        InitDepthMap();
    }
    else
    {
        return;
    }
}
void ShadowMapping::InitLightMatrice()
{
    Graphic::GlobalShaderParam* sp = Graphic::GlobalShaderParam::Get();
    sp->SubData("GlobalMatrices", 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(GetLightSpaceMatrice()));
}

void ShadowMapping::InitDepthMap()
{
    glEnable(GL_DEPTH_TEST);
    m_depthMapFrameBuffer.SetActive(true);
    m_depthMapFrameBuffer.SetShader("FrameBuffer/Depth");
    m_depthMapFrameBuffer.SetSize(m_shadowPrecision, m_shadowPrecision);

    m_depthMapFrameBuffer.AddTextureAttachment
    (
        FrameBuffer::Depth, 
        Graphic::Texture::DepthComponent, 
        Graphic::Texture::Depth, 
        Graphic::Texture::Float
    );
    m_depthMapFrameBuffer.Init();

    _Render_Queue_->EnqueMesh(m_depthMapFrameBuffer.GetOutputMesh(), RenderQueue::Overlay);

    m_depthMapFrameBuffer.Bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    m_depthMapFrameBuffer.UnBind();
    m_depthShader = new Graphic::Shader("ShadowMapping/DepthMap");
}

void ShadowMapping::SetUp(int width, int height, RenderQueue* rdq)
{
    m_width = width;
    m_height = height;
    _Render_Queue_ = rdq;
}

void ShadowMapping::SetLightView(const glm::vec3& from, const glm::vec3& to)
{
    m_view_from = from;
    m_view_to = to;
}

void ShadowMapping::SetLightProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near , GLfloat far)
{
    m_proj_left = left;
    m_proj_right = right;
    m_proj_top = top;
    m_proj_bottom = bottom;
    m_proj_near = near;
    m_proj_far = far;
}

glm::mat4 ShadowMapping::GetLightSpaceMatrice() const
{
    glm::mat4 proj(1.0f);
    if (m_proj_near != m_proj_far)
    {
        proj = glm::ortho(m_proj_left,m_proj_right,m_proj_bottom,m_proj_top,m_proj_near,m_proj_far);
    }
    else
    {
        proj = glm::ortho(m_proj_left,m_proj_right,m_proj_bottom,m_proj_top);
    }
    glm::mat4 view = glm::lookAt(m_view_from, m_view_to, glm::vec3(0,1,0));
    return proj * view;
}

void ShadowMapping::DepthPass()
{
    if (!m_enable)
        return;
    glViewport(0, 0, m_shadowPrecision, m_shadowPrecision);
    m_depthMapFrameBuffer.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    _Render_Queue_->Render(RenderQueue::Geometry, m_depthShader);
    m_depthMapFrameBuffer.UnBind();
}

void ShadowMapping::LightPass(GLuint targetFrameId)
{
    if (!m_enable)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, targetFrameId);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //_Render_Queue_->Render(RenderQueue::Geometry);
    //m_depthMapFrameBuffer.Draw();
}