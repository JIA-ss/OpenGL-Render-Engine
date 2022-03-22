#pragma once
#include "GraphicCommonHeader.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>
#include <unordered_map>
GRAPHIC_NAMESPACE_BEGIN

#define GENERATE_PARAM_TYPE(_CPP_TYPE_, _ENUM_)         \
    if (typeid(T) == typeid(_CPP_TYPE_))                \
    {                                                   \
        m_type = _ENUM_;                                \
        return;                                         \
    }

enum class ShaderParamType
{
    Vec2,
    Vec3,
    Vec4,
    Float,
    Double,
    Int,
    UInt,
    Bool,
    Mat2,
    Mat3,
    Mat4,
    Sampler2D,
    SamplerCube
};

class Texture;


class ShaderSetting
{
public:
    template <typename T>
    void AddParameter(const std::string &name, const T &val, const unsigned int& location);

    template <typename T>
    bool SetParameter(const std::string &name, const T &val);

    template <typename T>
    T *GetParameter(const std::string &name);

    void* GetRawValue(const std::string& name);

    void SetRawValue(const std::string& name, void* value);

    void UpdateParameters(Shader *shader);

    void Use() const;

    void SetTextures(const std::vector<Texture* >& texs);

    void Assign(ShaderSetting* ss) const;

    ShaderSetting Clone() const;
public:
    struct TextureParamValue
    {
        GLint texUnit;
        const Texture *texture;
        TextureParamValue(const GLint& id, const Texture* tex) : texUnit(id), texture(tex) { }
    };

protected:
    class ShaderParamBase
    {
    public:
        ShaderParamBase() = delete;
        ShaderParamBase(const ShaderParamBase &) = default;
        ShaderParamBase &operator=(const ShaderParamBase &) = default;
        ShaderParamBase(const unsigned int &loc, const ShaderParamType &type);
        ~ShaderParamBase() { }
        virtual void Set(void* val) = 0;
        virtual void* Get() = 0;
        virtual void Use() = 0;
        virtual std::unique_ptr<ShaderParamBase> Clone() = 0;
        void updateLocation(const unsigned int& loc);
        ShaderParamType GetType() const;
    protected:
        unsigned int m_loc;
        ShaderParamType m_type;
    };

    ShaderParamBase* GetRawParameter(const std::string& name);

    template <typename T>
    class ShaderParam : public ShaderParamBase
    {
    public:
        ShaderParam() = default;
        ShaderParam(const ShaderParam &) = default;
        ShaderParam &operator=(const ShaderParam &) = default;
        ShaderParam(const T &val, const unsigned int &loc, const ShaderParamType &type);
        ShaderParam(const T& val, const unsigned int &loc);
        ~ShaderParam() { }
        void SetValue(const T& val);
        T GetValue() const;
        virtual void Set(void* val) override;
        virtual void* Get() override;
        virtual void Use() override;
        virtual std::unique_ptr<ShaderParamBase> Clone() override;
    private:
        T m_val;
    };

private:
    std::unordered_map<std::string, std::unique_ptr<ShaderParamBase>> m_params;
};


template<typename T>
void ShaderSetting::AddParameter(const std::string &name, const T &val, const unsigned int& location)
{
    m_params.emplace(name, new ShaderParam<T>(val, location));
}

template<typename T>
bool ShaderSetting::SetParameter(const std::string &name, const T &val)
{
    auto it = m_params.find(name);
    if (it == m_params.end())
        return false;
    it->second->Set((void*)&val);
    return true;
}

template<typename T>
T* ShaderSetting::GetParameter(const std::string& name)
{
    auto it = m_params.find(name);
    if (it == m_params.end())
        return nullptr;
    return static_cast<T*>(it->second->Get());
}

template<typename T>
ShaderSetting::ShaderParam<T>::ShaderParam(const T& val, const unsigned int& loc)
{
    m_val = val;
    m_loc = loc;
    //GENERATE_PARAM_TYPE(int, ShaderParamType::Int);
    //GENERATE_PARAM_TYPE(bool, ShaderParamType::Bool);
    //GENERATE_PARAM_TYPE(glm::mat2, ShaderParamType::Mat2);
    //GENERATE_PARAM_TYPE(glm::mat3, ShaderParamType::Mat3);
    //GENERATE_PARAM_TYPE(glm::mat4, ShaderParamType::Mat4);
    //GENERATE_PARAM_TYPE(unsigned int, ShaderParamType::UInt);
    //GENERATE_PARAM_TYPE(float, ShaderParamType::Float);
    //GENERATE_PARAM_TYPE(double, ShaderParamType::Double);
    //GENERATE_PARAM_TYPE(glm::vec2, ShaderParamType::Vec2);
    //GENERATE_PARAM_TYPE(glm::vec3, ShaderParamType::Vec3);
    //GENERATE_PARAM_TYPE(glm::vec4, ShaderParamType::Vec4);
    // todo: Sample2D

}

template<typename T>
ShaderSetting::ShaderParam<T>::ShaderParam(const T &val, const unsigned int &loc, const ShaderParamType &type)
    : ShaderParamBase(loc, type), m_val(val) { }

template<typename T>
void ShaderSetting::ShaderParam<T>::SetValue(const T& val)
{
    m_val = val;
}

template<typename T>
T ShaderSetting::ShaderParam<T>::GetValue() const 
{
    return m_val;
}

template<typename T>
void ShaderSetting::ShaderParam<T>::Set(void* val)
{
    m_val = *static_cast<T*>(val);
}

template<typename T>
void* ShaderSetting::ShaderParam<T>::Get()
{
    return &m_val;
}

template<typename T>
void ShaderSetting::ShaderParam<T>::Use()
{
    Shader::setUniform(m_loc, m_val);
}


template<typename T>
std::unique_ptr<ShaderSetting::ShaderParamBase> ShaderSetting::ShaderParam<T>::Clone()
{
    return std::make_unique<ShaderSetting::ShaderParam<T>>(*this);
}

GRAPHIC_NAMESPACE_END