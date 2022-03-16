#include "StencilTest.h"

RENDER_NAMESPACE_USING

void StencilTest::Init()
{
    if (m_enable)
    {
        glEnable(GL_STENCIL_TEST);
        SetUp();
        glStencilOp(m_stencil_fail, m_depth_fail, m_depth_pass);
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }
}

void StencilTest::SetUp() const
{
    if (m_enable)
    {
        if (m_mask != 0x00)
            glStencilFunc(m_func, m_ref, 0xFF);
        glStencilMask(m_mask);
        if (m_mask != 0x00)
        {
            //glStencilOp(m_stencil_fail, m_depth_fail, m_depth_pass);
        }
    }
}

GLbitfield StencilTest::ClearMask() const
{
    if (m_enable)
        return GL_STENCIL_BUFFER_BIT;
    return 0;
}

void StencilTest::Update() const
{

}

void StencilTest::EnableWrite(bool v)
{
    if (v)
    {
        m_mask = 0xFF;
    }
    else
    {
        m_mask = 0x00;
    }
}

void StencilTest::SetFunc(StencilFunc func, GLint ref)
{
    m_func = func;
    m_ref = ref;
}

void StencilTest::SetOp(StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass)
{
    m_stencil_fail = stencil_fail;
    m_depth_fail = depth_fail;
    m_depth_pass = depth_pass;
}