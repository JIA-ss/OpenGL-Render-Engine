#pragma once
#include "FrameBuffer.h"
#include "render/RenderQueue.h"
RENDER_NAMESPACE_BEGIN

class ShadowMapping : public RenderControllerBase
{
public:
    ShadowMapping() { m_enable = false; }
    virtual void Init();
    Graphic::Shader* GetDepthShader() { return m_depthShader; }
    Graphic::Texture* GetDepthTexture();
public:
    void SetUp(int width, int height, RenderQueue* rdq);
    void SetLightProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near = 0, GLfloat far = 0);
    void SetLightView(const glm::vec3& from, const glm::vec3& to);
    glm::mat4 GetLightSpaceMatrice() const;
    void RenderToDepthBuffer();
    void RenderWithShadow(GLuint targetFrameId = 0);
private:
    void InitLightMatrice();
    void InitDepthMap();
private:
    GLuint m_shadowPrecision = 1024;
    FrameBuffer m_depthMapFrameBuffer;
    int m_width;
    int m_height;
    
    // for ortho
    GLfloat m_proj_left = -10, m_proj_right = 10, m_proj_top = 10, m_proj_bottom = -10;
    
    // common
    GLfloat m_proj_near = 1, m_proj_far = 15;
    glm::vec3 m_view_from = glm::vec3(-5.0f, 5.0f, 5.0f), m_view_to = glm::vec3(0.0f);

    Graphic::Shader* m_depthShader;
    Graphic::Material* m_depthMat;
    RenderQueue* _Render_Queue_;
};

RENDER_NAMESPACE_END