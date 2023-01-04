#pragma once
#include "GraphicCommonHeader.h"
#include "graphic/Material.h"
#include "graphic/Texture.h"
#include "graphic/Vertex.h"
#include "resource/types/TextureResource.h"
#include "GraphicBase.h"
#include <assimp/material.h>
#include <unordered_map>
GRAPHIC_NAMESPACE_BEGIN
using namespace Resource;

class Sprite : public GraphicBase
{
public:
    Sprite(Texture* tex, Material* mat) : m_tex(tex)
    {
        SetUpSprite();
    }
    void draw(Shader* shader = nullptr) const;
    void draw(Material* material, Shader* shader = nullptr) const;
private:
    void SetUpSprite();
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    Texture* m_tex;
};

GRAPHIC_NAMESPACE_END