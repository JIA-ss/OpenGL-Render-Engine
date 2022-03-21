#include "Material.h"
#include "system/ResourceSystem.h"
#include "ShaderSetting.h"
GRAPHIC_NAMESPACE_USING

std::unordered_map<std::string, Material> Material::collection;

Material::Material() : m_shader(nullptr){};

Material::Material(const unsigned int&renderIndex):Material()
{
    m_renderIndex = renderIndex;
}


void Material::SetShader(const std::string &name)
{
    m_shader = Shader::Add(name);
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

Material* Material::Clone() const
{
    Material* mat = new Material();
    mat->m_name = m_name;
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