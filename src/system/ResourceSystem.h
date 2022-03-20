#pragma once
#include "System.h"
#include "render/DepthTest.h"
#include "render/StencilTest.h"
#include "render/Blend.h"
#include "render/FaceCulling.h"
#include "render/FrameBuffer.h"
#include "render/RenderQueue.h"
#include "render/ShadowMapping.h"


class ResourceSystem : public System
{
    SYSTEM_STATIC_TYPE(ResourceSystem, System::Type::ResourceSystem)
public:
    ResourceSystem() : System(System::Type::ResourceSystem) { }
};