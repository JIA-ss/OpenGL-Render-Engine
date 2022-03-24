#pragma once

#include "RenderControllerBase.h"

#include "FrameBuffer.h"
RENDER_NAMESPACE_BEGIN

class PostProcessing : public RenderControllerBase
{
public:
    PostProcessing() { m_enable = false; }
    void Init() override;
    void SetUp() const override;
public:
    void PrepareRenderToTexture();
    void TextureToScreen();
    GLuint GetBufferId() const;
private:
    FrameBuffer m_buffer;
};


RENDER_NAMESPACE_END