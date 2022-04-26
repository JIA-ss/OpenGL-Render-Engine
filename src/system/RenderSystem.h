#pragma once
#include "System.h"
#include "entity/Camera.h"

#include "render/RenderQueue.h"

#include "render/controller/PostProcessing.h"
#include "render/controller/FrameBuffer.h"
#include "render/controller/ShadowMapping.h"

#include "render/RenderPath/ForwardRendering.h"
#include "render/RenderPath/DeferredRendering.h"

class RenderSystem : public System
{
    SYSTEM_STATIC_TYPE(RenderSystem, System::Type::RenderSystem)
public:
    RenderSystem() : System(System::Type::RenderSystem) { }
    void Init() override;
    void Update() override;

public:
    inline Entity::Camera& getCamera() { return *m_mainCamera; }
    inline Render::RenderQueue& getRenderQueue() { return m_renderQueue; }

    void SetRenderPath(Render::RenderPath path);
    Render::RenderPath GetCurRenderPath() const;

    Render::ForwardRendering& GetForwardRendering() { return m_forward; }
    Render::DeferredRendering& GetDeferredRendering() { return m_deferred; }

    inline bool UseRayTracing() const { return m_switchRayTracing; }
private:
    Entity::Camera* m_mainCamera = nullptr;
    Render::RenderPath m_renderPath;

    Render::ForwardRendering m_forward;
    Render::DeferredRendering m_deferred;

    Render::RenderQueue m_renderQueue;
    bool m_switchRayTracing = true;
};