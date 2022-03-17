#pragma once
#include "RenderControllerBase.h"
#include "graphic/Texture.h"
#include "graphic/Mesh.h"

#include <unordered_map>
RENDER_NAMESPACE_BEGIN

class RenderBuffer
{
public:
    enum InternalFormat
    {
        Depth24_Stencil8 = GL_DEPTH24_STENCIL8,
        Depth32_Stencil8 = GL_DEPTH32F_STENCIL8,
    };
public:
    RenderBuffer() = delete;
    RenderBuffer(const int& width, const int& height, const InternalFormat& InternalFormat = Depth24_Stencil8);
    ~RenderBuffer();

public:
    GLuint GetId() const { return m_renderBufferId; }
private:
    GLuint m_renderBufferId = 0;
};



class FrameBuffer: public RenderControllerBase
{
public:
    enum AttachmentType
    {
        Color0 = GL_COLOR_ATTACHMENT0,
        Color1 = GL_COLOR_ATTACHMENT1,
        Color2 = GL_COLOR_ATTACHMENT2,

        Depth = GL_DEPTH_ATTACHMENT,
        Stencil = GL_STENCIL_ATTACHMENT,
        DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
    };
    
    enum BufferType
    {
        Texture = 0,
        Render = 1
    };

public:
    FrameBuffer() { m_enable = false; }
    ~FrameBuffer();
public:
    void Init() override;
    void SetUp() const override; 
    GLbitfield ClearMask() const override;
    void Update() const override;
    void Bind() const;
    void UnBind() const;
    void Draw() const;
public:
    void SetSize(int width, int height);
    inline void SetShader(const std::string& shader) { m_shaderPath = shader; }
    bool IsValid() const;
    GLuint GetId() const { return m_frameBufferId; }

    Graphic::Texture* GetTextureAttachment(const AttachmentType& type);
    Graphic::Texture* AddTextureAttachment(
        const AttachmentType& type, 
        const Graphic::Texture::InternalFormat& internalFormat = Graphic::Texture::RGBInternal,
        const Graphic::Texture::DataFormat& dataFormat = Graphic::Texture::RGB,
        const Graphic::Texture::DataType& dataType = Graphic::Texture::UnsignedByte);

    RenderBuffer* GetRenderBuffer(const AttachmentType& type);
    RenderBuffer* AddRenderBuffer(const AttachmentType& type, const RenderBuffer::InternalFormat& format = RenderBuffer::Depth24_Stencil8);

protected:
    virtual void InitOutputMesh();
private:
    std::unordered_map<AttachmentType, Graphic::Texture* > m_textureBuffers;
    std::unordered_map<AttachmentType, RenderBuffer* > m_renderBuffers;
    int m_width;
    int m_height;
    GLuint m_frameBufferId = 0;
    Graphic::Mesh* m_outputMesh = nullptr;
    std::string m_shaderPath = "";
};

RENDER_NAMESPACE_END