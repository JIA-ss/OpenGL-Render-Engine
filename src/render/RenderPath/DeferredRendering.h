#pragma once

#include "RenderPathBase.h"
#include "render/controller/FrameBuffer.h"

RENDER_NAMESPACE_BEGIN

class DeferredRendering : public RenderPathBase
{
public:
    void InitForwardPath() override {}
    void InitDeferredPath() override;
    void InitMixesPath() override {}

public:
    FrameBuffer& GetGBuffer() { return m_gBuffer; }

    Graphic::Texture* GetPosTexture() { return m_gPosTex; }
    Graphic::Texture* GetNormalTexture() { return m_gNormalTex; }
    Graphic::Texture* GetColorTexture() { return m_gColorTex; }
private:
    void InitGBuffer(bool active);
    void GeomertyPass();
    void LightPass();
private:
    FrameBuffer m_gBuffer;

    Graphic::Shader* m_gShader;
    Graphic::Shader* m_lShader;

    Graphic::Mesh* m_outputMesh;

    Graphic::Texture* m_gPosTex;
    Graphic::Texture* m_gNormalTex;
    Graphic::Texture* m_gColorTex;
};

RENDER_NAMESPACE_END