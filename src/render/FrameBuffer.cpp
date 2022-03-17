#include "FrameBuffer.h"
#include "resource/resourceManager.h"

RENDER_NAMESPACE_USING

FrameBuffer::~FrameBuffer()
{
    if (m_frameBufferId != 0)
        glDeleteFramebuffers(1, &m_frameBufferId);
    for (auto it = m_textureBuffers.begin(); it != m_textureBuffers.end(); it++)
    {
        delete it->second;
    }

    for (auto it = m_renderBuffers.begin(); it != m_renderBuffers.end(); it++)
    {
        delete it->second;
    }

    delete m_outputMesh;
}

void FrameBuffer::Init()
{
    if (m_enable)
    {
        if (m_frameBufferId != 0)
            glDeleteFramebuffers(1, &m_frameBufferId);
        glGenFramebuffers(1, &m_frameBufferId);
        SetUp();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        InitOutputMesh();
        //glPolygonMode(GL_BACK, GL_LINE);
    }
    else
    {
        if (m_frameBufferId != 0)
            glDeleteFramebuffers(1, &m_frameBufferId);
    }
}
void FrameBuffer::SetUp() const
{
    if (!m_enable)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    for (auto&&[attachType, texture] : m_textureBuffers)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, GL_TEXTURE_2D, texture->GetId(), 0);
    }
    for (auto&&[attachType, render] : m_renderBuffers)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachType, GL_RENDERBUFFER, render->GetId());
    }
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void FrameBuffer::Bind() const
{
    if (!IsValid())
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    if (m_renderBuffers.find(DepthStencil) != m_renderBuffers.end())
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        return;
    }
    if (m_renderBuffers.find(Depth) != m_renderBuffers.end())
        glEnable(GL_DEPTH_TEST);
    if (m_renderBuffers.find(Stencil) != m_renderBuffers.end())
        glEnable(GL_STENCIL_TEST);
}

void FrameBuffer::UnBind() const
{
    if (!IsValid())
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

GLbitfield FrameBuffer::ClearMask() const
{
    return 0;
}
void FrameBuffer::Update() const
{

}

void FrameBuffer::Draw() const
{
    if (!IsValid())
        return;
    m_outputMesh->draw();
}

void FrameBuffer::SetSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

bool FrameBuffer::IsValid() const
{
    return m_valid && m_outputMesh != nullptr && m_enable;
}

static Graphic::TextureType Convert(const FrameBuffer::AttachmentType& type)
{
    switch (type)
    {
    case FrameBuffer::Depth:
        return Graphic::DepthAttachment;
    case FrameBuffer::Stencil:
        return Graphic::StencilAttachment;
    case FrameBuffer::DepthStencil:
        return Graphic::DepthStencilAttachment;
    default:
        if (type >= FrameBuffer::Color0 && type < FrameBuffer::Depth)
            return Graphic::ColorAttachment;
        return Graphic::Unknown;
    }
}

Graphic::Texture* FrameBuffer::GetTextureAttachment(const AttachmentType& type)
{
    auto it = m_textureBuffers.find(type);
    if (it == m_textureBuffers.end())
        return nullptr;
    return it->second;
}
Graphic::Texture* FrameBuffer::AddTextureAttachment(        
        const AttachmentType& type, 
        const Graphic::Texture::InternalFormat& internalFormat,
        const Graphic::Texture::DataFormat& dataFormat,
        const Graphic::Texture::DataType& dataType)
{
    auto it = m_textureBuffers.find(type);
    if (it == m_textureBuffers.end())
    {
        Graphic::Texture* texture = new Graphic::Texture(Convert(type), m_width, m_height,internalFormat, dataFormat, dataType);
        m_textureBuffers[type] = texture;
        return texture;
    }
    else
    {
        return it->second;
    }
}

RenderBuffer* FrameBuffer::GetRenderBuffer(const AttachmentType& type)
{
    auto it = m_renderBuffers.find(type);
    if (it == m_renderBuffers.end())
        return nullptr;
    return it->second;
}

RenderBuffer* FrameBuffer::AddRenderBuffer(const AttachmentType& type, const RenderBuffer::InternalFormat& format)
{
    auto it = m_renderBuffers.find(type);
    if (it == m_renderBuffers.end())
    {
        RenderBuffer* render = new RenderBuffer(m_width, m_height, format);
        m_renderBuffers[type] = render;
        return render;
    }
    else
    {
        return it->second;
    }
}

void FrameBuffer::InitOutputMesh()
{
    std::vector<Graphic::Texture*> texs;
    for (auto&&[attachType, texture] : m_textureBuffers)
    {
        texs.push_back(texture);
    }

    if (m_shaderPath.empty() || Resource::ResourceManager::Instance()->GetResource(m_shaderPath.c_str(), Resource::shader).isNull())
        m_shaderPath = "FrameBuffer/Default";

    Graphic::Material* mat = new Graphic::Material(m_shaderPath, texs);

    m_outputMesh = new Graphic::Mesh(Graphic::Vertex::quadElement, Graphic::Vertex::quad, mat, "FrameBuffer");
}



RenderBuffer::RenderBuffer(const int& width, const int& height, const InternalFormat& InternalFormat)
{
    glGenRenderbuffers(1, &m_renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId); 
    glRenderbufferStorage(GL_RENDERBUFFER, InternalFormat, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
RenderBuffer::~RenderBuffer()
{
    if (m_renderBufferId != 0)
    {
        glDeleteRenderbuffers(1, &m_renderBufferId);
    }
}