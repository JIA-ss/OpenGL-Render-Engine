#include "FrameBuffer.h"

RENDER_NAMESPACE_BEGIN

class ShadowMapping : public RenderControllerBase
{
public:
    virtual void Init();

public:
    void SetSize(int width, int height);
    void SetLightProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near = 0, GLfloat far = 0);
    void SetLightView(const glm::vec3& from, const glm::vec3& to);
    glm::mat4 GetLightSpaceMatrice() const;
    void DepthPass();
    void LightPass();
private:
    void InitDepthMap();
private:
    GLuint m_shadowPrecision = 1024;
    FrameBuffer m_depthMapFrameBuffer;
    int m_width;
    int m_height;
    
    glm::mat4 m_lightProjection;
    glm::mat4 m_lightView;
};

RENDER_NAMESPACE_END