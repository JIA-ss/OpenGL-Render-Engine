#pragma once
#include "GraphicCommonHeader.h"
#include <string>
#include <unordered_map>
#include <vector>
GRAPHIC_NAMESPACE_BEGIN

class GlobalShaderParam 
{
public:
    ~GlobalShaderParam();
    
    /**
     * @brief create a uniform buffer with name
     */
    GLuint GenBlock(const std::string& name, const GLsizeiptr& size, const void* data);
    void SubData(const GLuint& UBO, const GLintptr& offset, const GLsizeiptr& size, const void* data);
	void SubData(const std::string& name, const GLintptr& offset, const GLsizeiptr& size, const void* data);
    GLuint GetGlobalParameterIndex(const std::string& name);
    GLuint GetGlobalParameterUBO(const std::string& name);
    void ConfigureShaderParameterBlock(const GLuint& program);
    static GlobalShaderParam* Get();
private:
    GlobalShaderParam();
    struct ParamMeta
    {
        GLuint ubo;
        GLuint index;
        ParamMeta() = default;
        ParamMeta(GLuint _ubo, unsigned int _idx) : ubo(_ubo), index(_idx) {};
    };
    std::unordered_map<std::string,ParamMeta> m_params;

    static GlobalShaderParam* instance;
};

GRAPHIC_NAMESPACE_END