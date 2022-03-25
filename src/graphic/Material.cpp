#include "Material.h"
#include "system/ResourceSystem.h"
#include "ShaderSetting.h"
GRAPHIC_NAMESPACE_USING


GRAPHIC_IMPLEMENT(Material)

std::unordered_map<std::string, Material> Material::collection;

Material::Material(const std::string& shader, const std::vector<std::string>& textures, TextureType type) : Material(1)
{
    SetShader(shader);
    std::vector<Texture*> texes;
    for (auto&& tex : textures)
    {
        texes.push_back(new Texture(tex, type));
    }
    m_params.SetTextures(texes);
}

Material::Material() : m_shader(nullptr){};

Material::Material(const unsigned int&renderIndex):Material()
{
    m_renderIndex = renderIndex;
}


void Material::SetShader(const std::string &name)
{
    Shader* shader = ResourceSystem::GetGraphic<Shader>(name);
    if (shader == nullptr)
        shader = ResourceSystem::LoadGraphicResource<Shader>(name,name).GetGraphic();
    m_shader = shader;
    m_params.UpdateParameters(m_shader);
}

void Material::UseMaterial(Shader* shader) const
{
    if (shader != nullptr)
    {
        ShaderSetting param;
        param.UpdateParameters(shader);
        m_params.Assign(&param);
        shader->use();
        param.Use();
    }
    else
    {
        m_shader->use();
        m_params.Use();
    }
}

void Material::Free()
{
    // do nothing
        //because maybe other is referencing the shader
}

Material* Material::Clone() const
{
    auto matRes = ResourceSystem::LoadGraphicResource<Material>(m_name + "_copy");
    Material* mat = matRes.GetGraphic();
    mat->m_name = matRes.GetName();
    mat->m_shader = m_shader;
    mat->m_params = m_params.Clone();
    return mat;
}

Material *Material::Get(const std::string &id)
{
    auto iter = collection.find(id);
    if (iter == collection.end())
        return nullptr;
    return iter->second.Clone();
}

void Material::Remove(const std::string &id)
{
    collection.erase(id);
}