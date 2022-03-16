#pragma once
#include "RenderCommonHeader.h"

RENDER_NAMESPACE_BEGIN

class RenderControllerBase
{
public:
    virtual void Init() = 0;
    virtual void SetUp() const = 0; 
    virtual GLbitfield ClearMask() const = 0;
    virtual void Update() const = 0;
    void SetActive(bool v) { m_enable = v; }
    inline bool isActive() const { return m_enable; }
protected:
    bool m_enable;
};

RENDER_NAMESPACE_END