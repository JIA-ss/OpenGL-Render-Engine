#pragma once

#include "RenderControllerBase.h"

RENDER_NAMESPACE_BEGIN
// DepthTesting will execute after FragShader And Stencil Testing
class DepthTest : public RenderControllerBase
{
public:
    enum DepthFunc
    {
        Always = GL_ALWAYS,   //	永远通过深度测试
        Never = GL_NEVER,    //	永远不通过深度测试
        Less = GL_LESS,     //	在片段深度值小于缓冲的深度值时通过测试
        Equal = GL_EQUAL,    //	在片段深度值等于缓冲区的深度值时通过测试
        LEqual = GL_LEQUAL,   //	在片段深度值小于等于缓冲区的深度值时通过测试
        Greater = GL_GREATER,  //	在片段深度值大于缓冲区的深度值时通过测试
        NotEqual = GL_NOTEQUAL,     //	在片段深度值不等于缓冲区的深度值时通过测试
        GEqual = GL_GEQUAL,   //	在片段深度值大于等于缓冲区的深度值时通过测试
    };
public:
    DepthTest() = default;
public:
    void Init() const override;
    void SetUp() const override; 
    GLbitfield ClearMask() const override;
    void Update() const override;
    void SetFunc(DepthFunc func);
private:
    DepthFunc m_func = Less;
};


RENDER_NAMESPACE_END