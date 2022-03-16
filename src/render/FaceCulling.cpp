#include "FaceCulling.h"

RENDER_NAMESPACE_USING

void FaceCulling::Init()
{
    if (m_enable)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(m_cullingFace);
        glFrontFace(m_windingOrder);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}
void FaceCulling::SetUp() const
{

} 
GLbitfield FaceCulling::ClearMask() const
{
    return 0;
}
void FaceCulling::Update() const
{

}