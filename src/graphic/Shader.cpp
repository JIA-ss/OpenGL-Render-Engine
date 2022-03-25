#include "Shader.h"
#include "system/ResourceSystem.h"
#include "resource/resourceBasic.h"
#include "resource/types/ShaderResource.h"
#include <fstream>
#include <sstream>
#include <iostream>
GRAPHIC_NAMESPACE_USING

GRAPHIC_IMPLEMENT(Shader)

static unsigned int compileShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr)
{
    unsigned int shaderProgram, vertex, fragment, geometry;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);

    if (gShaderCode)
    {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
    }

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

    if (gShaderCode)
    {
        glCompileShader(geometry);
        glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
            std::cout << "Error::Shader::Geometry::Compile Faild\n" << infoLog << std::endl;
        }
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);

    if (gShaderCode)
    {
        glAttachShader(shaderProgram, geometry);
    }

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Error::Shader::Program::Link Faild\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (gShaderCode)
    {
        glDeleteShader(geometry);
    }
    return shaderProgram;
}
std::unordered_map<std::string, Shader*> Shader::collection = std::unordered_map<std::string, Shader*>();
Shader* Shader::Get(const std::string& name)
{
    auto it = collection.find(name);
    if (it == collection.end())
        return nullptr;
    return it->second;
}

Shader* Shader::Add(const std::string& name)
{
    auto it = collection.find(name);
    if (it != collection.end())
        return it->second;
    Shader* shader = new Shader(name);
    collection[name] = shader;
    return shader;
}

Shader::Shader(const std::string& name)
{
    Resource::ShaderRef vRef = ResourceSystem::Get()->GetResource((name + ".vs").c_str(), Resource::shader);
    Resource::ShaderRef fRef = ResourceSystem::Get()->GetResource((name + ".fs").c_str(), Resource::shader);
    Resource::ShaderRef gRef = ResourceSystem::Get()->GetResource((name + ".gs").c_str(), Resource::shader);

    if (vRef.isNull() || fRef.isNull())
    {
        std::string t = "false";
        assert(false);
    }

    const char* vertexCode = vRef->getContent();
    const char* fragmentCode = fRef->getContent();
    const char* geometryCode = gRef.isNull() ? nullptr : gRef->getContent();

    ID = compileShader(vertexCode, fragmentCode, geometryCode);
    GlobalShaderParam::Get()->ConfigureShaderParameterBlock(ID);
}

Shader::Shader(const char* vsName, const char* fsName)
{
    Resource::ShaderRef vRef = ResourceSystem::Get()->GetResource(vsName, Resource::shader);
    Resource::ShaderRef fRef = ResourceSystem::Get()->GetResource(fsName, Resource::shader);

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

void Shader::Free()
{
    glDeleteProgram(ID);
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