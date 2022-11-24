#pragma once

#include "Tracy.hpp"
#include "render/RenderCommonHeader.h"

#include <functional>
#include <vector>
RENDER_NAMESPACE_BEGIN

enum RenderPath
{
    Forward = 0,
    Deferred = 1,
    Mixed = 2
};

class RenderPathBase
{
public:

    virtual void InitForwardPath() = 0;
    virtual void InitDeferredPath() = 0;
    virtual void InitMixesPath() = 0;

    void Init(RenderPath path = Forward) 
    {
        ZoneScopedN("RenderPathBase::Init");
        switch (path)
        {
        case Forward:
            InitForwardPath();
            break;
        case Deferred:
            InitDeferredPath();
            break;
        case Mixed:
            InitMixesPath();
            break;
        default:
            break;
        }
    }

    void RenderPasses()
    {
        ZoneScopedN("RenderPathBase::RenderPasses");
        for (auto& pass : m_passes)
        {
            pass();
        }
    }
protected:
    std::vector<std::function<void()>> m_passes;
};

RENDER_NAMESPACE_END