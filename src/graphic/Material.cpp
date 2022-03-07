#include "Material.h"
#include "resource/resourceManager.h"
#include "ShaderSetting.h"
GRAPHIC_NAMESPACE_USING

std::unordered_map<std::string, Material> Material::collection;

Material::Material() : m_shader(nullptr), m_attachedMeshes(){};

Material::Material(const unsigned int&renderIndex):Material()
{
    m_renderIndex = renderIndex;
}

void Material::SetRenderIndex(const unsigned int& index)
{
    m_renderIndex = index;
}

void Material::SetShader(const std::string &name)
{
    m_shader = Shader::Add(name);
    m_params.UpdateParameters(m_shader);
}

void Material::UseMaterial() const
{
    m_shader->use();
    m_params.Use();
}

Material *Material::Get(const std::string &id)
{
    auto iter = collection.find(id);
    if (iter == collection.end())
        return nullptr;
    return &iter->second;
}

void Material::Remove(const std::string &id)
{
    collection.erase(id);
}