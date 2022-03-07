#pragma once
#include "GraphicCommonHeader.h"
#include "GlobalShaderParam.h"

#include <string>


GRAPHIC_NAMESPACE_BEGIN

class Shader
{
public:
    Shader(const char* vsName, const char* fsName);
    Shader(const Shader& other) 
    {
        ID = other.ID;
    }
    unsigned int ID;
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void setVec3(const std::string& name, float v1, float v2, float v3) const;
    void setMat3f(const std::string& name, const float* value) const;
    void setMat4f(const std::string& name, const float* value) const;

    template<typename T>
    static void setUniform(const GLuint &location, const T& val);

};




GRAPHIC_NAMESPACE_END