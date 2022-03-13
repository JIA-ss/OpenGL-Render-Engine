#pragma once
#include "RenderControllerBase.h"

RENDER_NAMESPACE_BEGIN

class FaceCulling: public RenderControllerBase
{
public:
    enum CullingFace
    {
       Back = GL_BACK, //只剔除背向面。
       Front = GL_FRONT, //只剔除正向面。
       FrontAndBack = GL_FRONT_AND_BACK //剔除正向面和背向面。
    };
    enum WindingOrder
    {
        ClockWise = GL_CW,
        CounterClockWise = GL_CCW
    };
public:
    FaceCulling() { m_enable = false; };
public:
    void Init() const override;
    void SetUp() const override; 
    GLbitfield ClearMask() const override;
    void Update() const override;

    inline void SetCullingFace(CullingFace face) { m_cullingFace = face; }
    inline void SetWindingOrder(WindingOrder order) { m_windingOrder = order; }
private:
    CullingFace m_cullingFace;
    WindingOrder m_windingOrder;
};

RENDER_NAMESPACE_END