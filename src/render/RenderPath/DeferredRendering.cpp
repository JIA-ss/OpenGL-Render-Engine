#include "DeferredRendering.h"
#include "engine/Engine.h"
#include "graphic/Texture.h"
#include "graphic/GlobalShaderParam.h"

#include "system/RenderSystem.h"
#include "system/ResourceSystem.h"

#include "entity/Entity.h"
#include "component/MeshRender.h"
RENDER_NAMESPACE_USING
GRAPHIC_NAMESPACE_USING


void DeferredRendering::InitDeferredPath()
{
    InitGBuffer(true);

    // geometry pass
    m_passes.push_back([this]()
    {
        this->GeomertyPass();
    });

    // light pass
    m_passes.push_back([this]()
    {
        this->LightPass();
    });

    // just for debug
    //Entity::sEntity* debugEntity = Entity::sEntity::Create<Entity::sEntity>();
    //Graphic::Mesh* mesh = ResourceSystem::LoadGraphicResource<Graphic::Mesh>("gBufDebugMes", VertexStream::quad, "DeferredRendering/gBufDebugOutput", std::vector<Graphic::Texture*>{m_gPosTex, m_gNormalTex,m_gColorTex}).GetGraphic();
    //debugEntity->AddComponent<Component::sMeshRender>(std::vector<Graphic::Mesh*>{mesh}, 4000);
}

void DeferredRendering::InitGBuffer(bool active)
{
    if (active)
    {
        // Set up G-Buffer
        m_gBuffer.SetActive(true);
        m_gBuffer.SetSize(Engine::GetWindowWidth(), Engine::GetWindowHeight());
        
        // positions(RGB)
        m_gPosTex = m_gBuffer.AddTextureAttachment(FrameBuffer::Color0, Texture::RGB16F, Texture::RGB, Texture::Float);

        // normal(RGB) 
        m_gNormalTex = m_gBuffer.AddTextureAttachment(FrameBuffer::Color1, Texture::RGB16F, Texture::RGB, Texture::Float);

        // Color (RGB) + Specular (A)
        m_gColorTex = m_gBuffer.AddTextureAttachment(FrameBuffer::Color2, Texture::RGBAInternal, Texture::RGBA, Texture::UnsignedByte);

        // depth render buffer
        m_gBuffer.AddRenderBuffer(FrameBuffer::Depth, RenderBuffer::DepthComponent);

        m_gBuffer.Init();
        
        m_gShader = ResourceSystem::LoadGraphicResource<Shader>("DeferredRendering/GPass","DeferredRendering/GPass").GetGraphic();

        m_outputMesh = ResourceSystem::LoadGraphicResource<Mesh>
                        (
                            "DeferredOutput- Mesh",
                            VertexStream::quad, "DeferredRendering/LightingPass", std::vector<Texture*>{m_gPosTex, m_gNormalTex, m_gColorTex}
                        ).GetGraphic();
    }
    else
    {
        m_gBuffer.SetActive(false);
    }
}


void DeferredRendering::GeomertyPass()
{
    m_gBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto& rq = RenderSystem::Get()->getRenderQueue();
    rq.Render(RenderQueue::Geometry, m_gShader);
    m_gBuffer.UnBind();
}

void DeferredRendering::LightPass()
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_outputMesh->draw();
    auto& rq = RenderSystem::Get()->getRenderQueue();
    rq.Render(RenderQueue::Overlay);
    rq.Render(RenderQueue::Background);
}