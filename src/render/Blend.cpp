#include "Blend.h"

RENDER_NAMESPACE_USING

void Blend::Init() const
{
    if (m_enable)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
        glDisable(GL_BLEND);
}
void Blend::SetUp() const
{

} 
GLbitfield Blend::ClearMask() const
{
    return 0;
}
void Blend::Update() const
{

}