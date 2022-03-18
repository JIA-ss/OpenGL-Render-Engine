#pragma once
#include "RenderCommonHeader.h"

RENDER_NAMESPACE_BEGIN

class RenderControllerBase
{
public:
    virtual void Init() = 0;
    virtual void SetUp() const {}; 
    virtual GLbitfield ClearMask() const { return 0; };
    virtual void Update() const {};
    void SetActive(bool v) { m_enable = v; }
    inline bool isActive() const { return m_enable; }
protected:
    bool m_enable;
};

RENDER_NAMESPACE_END