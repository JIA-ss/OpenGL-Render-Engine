#include "DepthTest.h"

RENDER_NAMESPACE_USING

void DepthTest::Init()
{
    if (m_enable)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc((GLenum)m_func);
    }
    else
    {
        //glDisable(GL_DEPTH_TEST);
    }
}

void DepthTest::SetUp() const 
{
    
}

GLbitfield DepthTest::ClearMask() const 
{
    if (m_enable)
    {
        return GL_DEPTH_BUFFER_BIT;
    }
    return 0;
}

void DepthTest::SetFunc(DepthFunc func)
{
    m_func = func;
}

void DepthTest::Update() const
{
    if (!m_enable)
        return;
}