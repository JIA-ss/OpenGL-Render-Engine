#pragma once

#include "common/headers.h"

#include <string>
#include <fstream>
#include <sstream>

namespace Pipline{


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
    void setMat4f(const std::string& name, const float* value) const;
};


}