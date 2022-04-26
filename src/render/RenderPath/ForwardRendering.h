#pragma once

#include "RenderPathBase.h"

#include "render/controller/FrameBuffer.h"
#include "render/controller/ShadowMapping.h"
#include "render/controller/PostProcessing.h"

#include "render/controller/RayTracing.h"
RENDER_NAMESPACE_BEGIN

class ForwardRendering : public RenderPathBase
{
public:
    void InitForwardPath() override;
    void InitDeferredPath() override;
    void InitMixesPath() override;

public:
    PostProcessing& GetPostProcessingController() { return m_postProcessing; }
    ShadowMapping& GetShadowMapingController() { return m_shadowMapping; }
    RayTracing& GetRayTracingController() { return m_rayTracing; }
private:
    PostProcessing m_postProcessing;
    ShadowMapping m_shadowMapping;
    RayTracing m_rayTracing;
};

RENDER_NAMESPACE_END