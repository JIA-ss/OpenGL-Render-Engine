#include "ShaderSetting.h"
#include <iostream>
GRAPHIC_NAMESPACE_USING




unsigned int ShaderSetting::GenerateLocation()
{
    static unsigned int location = 0;
    return location++;
}

void ShaderSetting::Use()
{
    for (auto&& [name, param] : m_params)
    {
        param->Use();
    }
}

template <>
void ShaderSetting::ShaderParam<ShaderSetting::TextureParamValue>::Use()
{
    glActiveTexture(GL_TEXTURE0 + m_val.texUnit); 
    if(m_val.texture)
        glBindTexture(GL_TEXTURE_2D, m_val.texture->GetId());
    else 
        glBindTexture(GL_TEXTURE_2D, 0);
	Shader::setUniform(m_loc, m_val.texUnit);
}



#define MAKE_CASE(glType, storeType, enumType, glFunc)                                                                  \
    case glType:                                                                                                        \
        for (int s = 0; s < size; s++)                                                                                  \
        {                                                                                                               \
            if (s > 0)                                                                                                  \
            {                                                                                                           \
                nameStr = nameStr.substr(0, nameStr.find_last_of("["));                                                 \
                nameStr += "[" + std::to_string(s) + "]";                                                               \
            }                                                                                                           \
            auto loc = glGetUniformLocation(program, nameStr.data());                                                   \
            if (loc != GL_INVALID_INDEX)                                                                                \
            {                                                                                                           \
                storeType val;                                                                                          \
                glFunc(program, loc, &val);                                                                             \
                m_params.emplace(nameStr, new ShaderParam<storeType>(val, loc, ShaderParamType::enumType));             \
            }                                                                                                           \
        }                                                                                                               \
        break;  
    
#define MAKE_CASE_VM(glType, storeType, enumType, glFunc)                                                               \
    case glType:                                                                                                        \
        for (int s = 0; s < size; s++)                                                                                  \
        {                                                                                                               \
            if (s > 0)                                                                                                  \
            {                                                                                                           \
                nameStr = nameStr.substr(0, nameStr.find_last_of("["));                                                 \
                nameStr += "[" + std::to_string(s) + "]";                                                               \
            }                                                                                                           \
            auto loc = glGetUniformLocation(program, nameStr.data());                                                   \
            if (loc != GL_INVALID_INDEX)                                                                                \
            {                                                                                                           \
                storeType val;                                                                                          \
                glFunc(program, loc, glm::value_ptr(val));                                                              \
                m_params.emplace(nameStr, new ShaderParam<storeType>(val, loc, ShaderParamType::enumType));             \
            }                                                                                                           \
        }                                                                                                               \
        break;


void ShaderSetting::UpdateParameters(Shader* shader)
{
    m_params.clear();
    auto program = shader->ID;
    const GLsizei buffSize = 256;
    GLint count;
    GLchar name[buffSize];
    GLsizei length;
    GLenum type;
    GLint size;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++)
    {
        glGetActiveUniform(program, (GLuint)i, buffSize, &length, &size, &type, name);
        std::string nameStr = std::string(name);
        switch (type)
        {
            MAKE_CASE(GL_FLOAT, float, Float, glGetUniformfv);
            MAKE_CASE(GL_DOUBLE, double, Double, glGetUniformdv);
            MAKE_CASE(GL_INT, int, Int, glGetUniformiv);
            MAKE_CASE(GL_UNSIGNED_INT, unsigned int, UInt, glGetUniformuiv);
            MAKE_CASE(GL_BOOL, int, Bool, glGetUniformiv);
            MAKE_CASE_VM(GL_FLOAT_VEC2, glm::vec2, Vec2, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_VEC3, glm::vec3, Vec3, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_VEC4, glm::vec4, Vec4, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_MAT2, glm::mat2, Mat2, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_MAT3, glm::mat3, Mat3, glGetUniformfv);
            MAKE_CASE_VM(GL_FLOAT_MAT4, glm::mat4, Mat4, glGetUniformfv);
            case GL_SAMPLER_2D:
                for (int s = 0; s < size; s++)
                {
                    if (s > 0)
                    {
                        nameStr = nameStr.substr(0, nameStr.find_last_of("["));
                        nameStr += "[" + std::to_string(s) + "]";
                    }
                    auto loc = glGetUniformLocation(program, nameStr.data());
                    if (loc != GL_INVALID_INDEX)
                    {
                        GLint val;
                        glGetUniformiv(program, loc, &val);
                        TextureParamValue actualVal(val, nullptr);
                        m_params.emplace(nameStr, new ShaderParam<TextureParamValue>(actualVal, loc, ShaderParamType::Sampler2D));
                    }
                }
                break;
        default:
            std::cerr << "type not supported in shader parameter: " << type << " " << name << std::endl;
            break;
        }
    }
}


