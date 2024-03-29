#pragma once
#include "GraphicCommonHeader.h"
#include "GlobalShaderParam.h"
#include "GraphicBase.h"
#include <string>


GRAPHIC_NAMESPACE_BEGIN
class ShaderSetting;
class Shader : public GraphicBase
{
    GRAPHIC_DECLARE
public:
    Shader(const std::string& name);
    Shader(const char* vsName, const char* fsName);
    Shader(const Shader& other) 
    {
        ID = other.ID;
        UpdateCommonSetting();
    }
    ~Shader();
    inline ShaderSetting* getCommonSetting() { return m_common_setting; }
    unsigned int ID;
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void setVec3(const std::string& name, float v1, float v2, float v3) const;
    void setMat3f(const std::string& name, const float* value) const;
    void setMat4f(const std::string& name, const float* value) const;

    void Free() override;

    template<typename T>
    static void setUniform(const GLuint &location, const T& val);
private:
    void UpdateCommonSetting();
    ShaderSetting* m_common_setting = nullptr;
public:
    static Shader* Get(const std::string& name);
    static Shader* Add(const std::string& name);
private:
    static std::unordered_map<std::string, Shader*> collection;
};




GRAPHIC_NAMESPACE_END