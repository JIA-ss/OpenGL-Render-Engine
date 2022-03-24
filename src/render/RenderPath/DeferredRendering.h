#pragma once

#include "RenderPathBase.h"


RENDER_NAMESPACE_BEGIN

class DeferredRendering : public RenderPathBase
{
public:
    void InitForwardPath() override { }

    void InitDeferredPath() override { }

    void InitMixesPath() override { }
};

RENDER_NAMESPACE_END