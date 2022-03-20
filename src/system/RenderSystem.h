#pragma once
#include "System.h"
#include "entity/Camera.h"

#include "render/DepthTest.h"
#include "render/StencilTest.h"
#include "render/Blend.h"
#include "render/FaceCulling.h"
#include "render/FrameBuffer.h"
#include "render/RenderQueue.h"
#include "render/ShadowMapping.h"
class RenderSystem : public System
{
    SYSTEM_STATIC_TYPE(RenderSystem, System::Type::RenderSystem)
public:
    RenderSystem() : System(System::Type::RenderSystem) { }
    void Init() override;
    void Update() override;



public:
    inline Entity::Camera& getCamera() { return *m_mainCamera; }
    inline Render::DepthTest& getDepthTest() { return m_depthTest; }
    inline Render::StencilTest& getStencilTest() { return m_stencilTest; }
    inline Render::Blend& getBlend() { return m_blend; }
    inline Render::FaceCulling& getFaceCulling() { return m_faceCulling; }
    inline Render::FrameBuffer& getFrameBuffer() { return m_frameBuffer; }
    inline Render::RenderQueue& getRenderQueue() { return m_renderQueue; }
    inline Render::ShadowMapping& getShadowMapping() { return m_shadowMapping; }
private:
    Entity::Camera* m_mainCamera = nullptr;

    Render::DepthTest m_depthTest;
    Render::StencilTest m_stencilTest;
    Render::Blend m_blend;
    Render::FaceCulling m_faceCulling;
    Render::FrameBuffer m_frameBuffer;
    Render::RenderQueue m_renderQueue;
    Render::ShadowMapping m_shadowMapping;
};