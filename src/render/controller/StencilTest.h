#pragma once
#include "RenderControllerBase.h"

RENDER_NAMESPACE_BEGIN

// StencilTesting will execute after FragShader
class StencilTest : public RenderControllerBase
{
public:
    enum StencilFunc
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
    enum StencilOp
    {
       Keep = GL_KEEP,  //	保持当前储存的模板值
       Zero = GL_ZERO,  //	将模板值设置为0
       Replace = GL_REPLACE,   //	将模板值设置为glStencilFunc函数设置的ref值
       Incr = GL_INCR,  //	如果模板值小于最大值则将模板值加1
       Incr_Wrap = GL_INCR_WRAP, //	与GL_INCR一样，但如果模板值超过了最大值则归零
       Decr = GL_DECR,  //	如果模板值大于最小值则将模板值减1
       Decr_Wrap = GL_DECR_WRAP, //	与GL_DECR一样，但如果模板值小于0则将其设置为最大值
       Invert = GL_INVERT,    //	按位翻转当前的模板缓冲值
    };
public:
    StencilTest() = default;
public:
    void Init() override;
    void SetUp() const override; 
    GLbitfield ClearMask() const override;
    void Update() const override;
    void EnableWrite(bool v);
    void SetFunc(StencilFunc func, GLint ref);
    void SetOp(StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass);
private:
    GLuint m_mask = 0xFF;

    StencilFunc m_func;
    GLint m_ref;

    StencilOp m_stencil_fail = Keep;
    StencilOp m_depth_fail = Keep;
    StencilOp m_depth_pass = Keep;
};

RENDER_NAMESPACE_END