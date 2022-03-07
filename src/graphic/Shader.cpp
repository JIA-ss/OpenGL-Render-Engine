#include "Shader.h"
#include "resource/resourceManager.h"
#include "resource/resourceBasic.h"
#include "resource/types/ShaderResource.h"
#include <fstream>
#include <sstream>
GRAPHIC_NAMESPACE_USING


static unsigned int compileShader(const char* vShaderCode, const char* fShaderCode)
{
    unsigned int shaderProgram, vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);

    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "Error::Shader::Vertex::Compile Faild\n" << infoLog << std::endl;
    }

    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "Error::Shader::Fragment::Compile Faild\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Error::Shader::Program::Link Faild\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}

Shader::Shader(const char* vsName, const char* fsName)
{
    Resource::ShaderRef vRef = Resource::ResourceManager::Instance()->GetResource(vsName, Resource::shader);
    Resource::ShaderRef fRef = Resource::ResourceManager::Instance()->GetResource(fsName, Resource::shader);

    if (vRef.isNull() || fRef.isNull())
    {
        std::string t = "false";
        assert(false);
    }

    const char* vertexCode = vRef->getContent();
    const char* fragmentCode = fRef->getContent();
    
    //std::cout << "vertex shader:" << std::endl << vertexCode << std::endl << " frag shader: " << std::endl << fragmentCode << std::endl;

    //编译着色器
    ID = compileShader(vertexCode, fragmentCode);

    GlobalShaderParam::Get()->ConfigureShaderParameterBlock(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec4(const std::string& name, float v1, float v2, float v3, float v4) const 
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::setVec3(const std::string& name, float v1, float v2, float v3) const 
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setMat3f(const std::string& name, const float* value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMat4f(const std::string& name, const float* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}


template<typename T>
void Shader::setUniform(const GLuint &location, const T& val)
{

}

#define SET_UNIFORM_BASE_TYPE_SPECIALIZE(_CLASS_, _GL_FUNC_)                                    \
template<> void Shader::setUniform<_CLASS_>(const GLuint& location, const _CLASS_& val)         \
{                                                                                               \
    _GL_FUNC_(location, val);                                                                   \
}

#define SET_UNIFORM_VEC_TYPE_SPECIALIZE(_CLASS_, _GL_FUNC_)                                     \
template<> void Shader::setUniform<_CLASS_>(const GLuint& location, const _CLASS_& val)         \
{                                                                                               \
    _GL_FUNC_(location, 1, &val[0]);                                                            \
}

#define SET_UNIFORM_MAT_TYPE_SPECIALIZE(_CLASS_, _GL_FUNC_)                                     \
template<> void Shader::setUniform<_CLASS_>(const GLuint& location, const _CLASS_& val)         \
{                                                                                               \
    _GL_FUNC_(location, 1, GL_FALSE, &val[0][0]);                                                            \
}


SET_UNIFORM_BASE_TYPE_SPECIALIZE(float, glUniform1f)
SET_UNIFORM_BASE_TYPE_SPECIALIZE(int, glUniform1i)
SET_UNIFORM_BASE_TYPE_SPECIALIZE(unsigned int, glUniform1ui)
SET_UNIFORM_BASE_TYPE_SPECIALIZE(bool, glUniform1i)
SET_UNIFORM_BASE_TYPE_SPECIALIZE(double, glUniform1d)

SET_UNIFORM_VEC_TYPE_SPECIALIZE(glm::vec2, glUniform2fv)
SET_UNIFORM_VEC_TYPE_SPECIALIZE(glm::vec3, glUniform3fv)
SET_UNIFORM_VEC_TYPE_SPECIALIZE(glm::vec4, glUniform4fv)

SET_UNIFORM_MAT_TYPE_SPECIALIZE(glm::mat2, glUniformMatrix2fv)
SET_UNIFORM_MAT_TYPE_SPECIALIZE(glm::mat3, glUniformMatrix3fv)
SET_UNIFORM_MAT_TYPE_SPECIALIZE(glm::mat4, glUniformMatrix4fv)