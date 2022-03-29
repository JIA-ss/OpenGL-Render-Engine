#include "GlobalShaderParam.h"

GRAPHIC_NAMESPACE_USING

GlobalShaderParam::GlobalShaderParam()
{
}
GlobalShaderParam* GlobalShaderParam::instance = nullptr;
GlobalShaderParam *GlobalShaderParam::Get()
{
    if (instance)
    {
        return instance;
    }
    instance = new GlobalShaderParam();
    instance->GenBlock("GlobalMatrices", 3 * sizeof(glm::mat4), nullptr);
    instance->GenBlock("GlobalPositions", 3 * sizeof(glm::vec4), nullptr);
    instance->GenBlock("LightPositions", 5 * sizeof(glm::vec4), nullptr);
    instance->GenBlock("LightColors", 5 * sizeof(glm::vec4), nullptr);
    return instance;
}

GLuint GlobalShaderParam::GenBlock(const std::string& name, const GLsizeiptr &size, const void* data)
{
    assert(m_params.find(name) == m_params.end());

    static unsigned int index = 0;
	GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, index, UBO);
	m_params[name] = { UBO, index++ };
    return UBO;
}

void GlobalShaderParam::SubData(const GLuint& UBO, const GLintptr &offset, const GLsizeiptr &size, const void *data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderParam::SubData(const std::string& name, const GLintptr& offset, const GLsizeiptr& size, const void* data)
{
    auto it = m_params.find(name);
    assert(it != m_params.end());
    GLuint UBO = it->second.ubo;
    SubData(UBO, offset, size, data);
}

void GlobalShaderParam::ConfigureShaderParameterBlock(const GLuint& program)
{
    for (auto& [name, meta] : m_params)
    {
        auto uniformBlockIndex = glGetUniformBlockIndex(program, name.data());
        if (uniformBlockIndex != GL_INVALID_INDEX)
            glUniformBlockBinding(program, uniformBlockIndex, meta.index);
    }
}

GLuint GlobalShaderParam::GetGlobalParameterIndex(const std::string& name)
{
    auto it = m_params.find(name);
    assert(it != m_params.end());
    return it->second.index;
}

GLuint GlobalShaderParam::GetGlobalParameterUBO(const std::string& name)
{
    auto it = m_params.find(name);
    assert(it != m_params.end());
    return it->second.ubo;
}