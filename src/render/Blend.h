#pragma once
#include "RenderControllerBase.h"

RENDER_NAMESPACE_BEGIN

class Blend : public RenderControllerBase
{
public:
    Blend() { m_enable = true; };
public:
    void Init() const override;
    void SetUp() const override; 
    GLbitfield ClearMask() const override;
    void Update() const override;
};

RENDER_NAMESPACE_END