#include "ForwardRendering.h"
#include "system/RenderSystem.h"
#include "engine/Engine.h"
RENDER_NAMESPACE_USING

void ForwardRendering::InitForwardPath()
{
    ZoneScopedN("ForwardRendering::InitForwardPath");
    m_postProcessing.SetActive(true);
    m_shadowMapping.SetActive(true);

    m_postProcessing.Init();
    m_shadowMapping.Init();
    m_shadowMapping.SetUp(Engine::GetWindowWidth(), Engine::GetWindowHeight(), &RenderSystem::Get()->getRenderQueue());

    if (RenderSystem::Get()->UseRayTracing())
    {
        m_passes.push_back([=](){
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0,0,0,1);
            GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
            glClear(mask);
            this->GetRayTracingController().Update();
        });
        return;
    }

    // 1st pass
    m_passes.push_back([this]()
    {
        ZoneScopedN("ForwardRendering::DepthPass");
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
        ZoneScopedN("ForwardRendering::LightPass");
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
