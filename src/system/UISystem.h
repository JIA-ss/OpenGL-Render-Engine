#pragma once
#include "System.h"
#include "entity/Camera.h"

#include "render/RenderQueue.h"

#include "render/controller/PostProcessing.h"
#include "render/controller/FrameBuffer.h"
#include "render/controller/ShadowMapping.h"

#include "render/RenderPath/ForwardRendering.h"
#include "render/RenderPath/DeferredRendering.h"

class UISystem : public System
{
    SYSTEM_STATIC_TYPE(UISystem, System::Type::UISystem)
public:
    UISystem() : System(System::Type::UISystem) { }
    void Init() override;
    void Update() override;
};