#pragma once
#include "GraphicCommonHeader.h"
#include "ShaderSetting.h"
#include "Texture.h"
#include "GraphicBase.h"
GRAPHIC_NAMESPACE_BEGIN

class Mesh;

class Shader;
class Material : public GraphicBase
{
    GRAPHIC_DECLARE
public:
    Material();
    Material(const unsigned int& renderIndex);
    Material(const std::string& shader, const std::vector<std::string>& textures, TextureType type = Diffuse);
    Material(const std::string &shader, const std::vector<Texture *>& textures) : Material(1)
    {
        SetShader(shader);
        m_params.SetTextures(textures);
    };
public:
    void SetShader(const std::string& name);
    void UseMaterial(Shader* shader = nullptr) const;
    const Shader* GetShader() const;
    ShaderSetting* GetShaderParams() const;
    void SetRenderIndex(const unsigned int& idx);
    void SetName(const std::string& name) { m_name = name; }
    const char* GetName() const { return m_name.c_str(); }

    template<typename T>
    bool SetShaderParam(const std::string& name, const T& val);

    Material* Clone() const;
    void Free() override;
protected:
    std::string m_name;
    Shader* m_shader;
    ShaderSetting m_params;
    unsigned int m_renderIndex;
public:
    static Material *Get(const std::string &id);
    template <typename... Args>
    static Material *Add(const std::string &id, Args &&... args);
    static void Remove(const std::string &id);
private:
    static std::unordered_map<std::string, Material> collection; 
};


template <typename... Args>
Material *Material::Add(const std::string &id, Args &&... args)
{
    auto has = collection.find(id);
    if (has != collection.end())
        return has->second.Clone();
    auto pair = collection.emplace(id, Material(args...));
    pair.first->second.SetName(id);
    return pair.first->second.Clone();
}

template<typename T>
bool Material::SetShaderParam(const std::string& name, const T& val)
{
    //unsigned int location = glGetUniformLocation(m_shader->ID, name.c_str());
    return m_params.SetParameter(name, val);
}

GRAPHIC_NAMESPACE_END