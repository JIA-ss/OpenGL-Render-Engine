#include "ForwardRendering.h"
#include "system/RenderSystem.h"
#include "engine/Engine.h"
RENDER_NAMESPACE_USING

void ForwardRendering::InitForwardPath()
{
    m_postProcessing.SetActive(true);
    m_shadowMapping.SetActive(true);

    m_postProcessing.Init();
    m_shadowMapping.Init();
    m_shadowMapping.SetUp(Engine::GetWindowWidth(), Engine::GetWindowHeight(), &RenderSystem::Get()->getRenderQueue());

    // 1st pass
    m_passes.push_back([this]()
    {
        auto& postProcess = this->GetPostProcessingController();
        postProcess.PrepareRenderToTexture();
        auto& rq = RenderSystem::Get()->getRenderQueue();
        //rq.Render(RenderQueue::Geometry);
        //rq.Render(RenderQueue::Transparent);
        this->GetShadowMapingController().RenderToDepthBuffer();
        this->GetShadowMapingController().RenderWithShadow(postProcess.GetBufferId()); 
        rq.Render(RenderQueue::Transparent);
    });

    //2st pass
    m_passes.push_back([this]()
    {
        this->GetPostProcessingController().TextureToScreen();
        auto& rq = RenderSystem::Get()->getRenderQueue();
        rq.Render(RenderQueue::Overlay);
    });
}

void ForwardRendering::InitDeferredPath()
{

}

void ForwardRendering::InitMixesPath()
{

}
