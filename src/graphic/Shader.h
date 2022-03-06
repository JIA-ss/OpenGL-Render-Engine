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


    static void setUniform(const GLuint &location, const glm::mat4 &mat)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }
    static void setUniform(const GLuint &location, const glm::mat3 &mat)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
    }
    static void setUniform(const GLuint &location, const glm::mat2 &mat)
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
    }
    static void setUniform(const GLuint &location, const glm::vec2 &vec)
    {
        glUniform2fv(location, 1, &vec[0]);
    }
    static void setUniform(const GLuint &location, const glm::vec3 &vec)
    {
        glUniform3fv(location, 1, &vec[0]);
    }
	static void setUniform(const GLuint& location, const glm::vec4& vec)
	{
		glUniform4fv(location, 1, &vec[0]);
	}
	static void setUniform(const GLuint& location, const float& x, const float& y)
	{
		glUniform2f(location, x, y);
	}
    static void setUniform(const GLuint &location, const float &x, const float &y, const float &z)
    {
        glUniform3f(location, x, y, z);
    }
    static void setUniform(const GLuint &location, const float &v)
    {
        glUniform1f(location, v);
    }
    static void setUniform(const GLuint &location, const int &v)
    {
        glUniform1i(location, v);
    }
    static void setUniform(const GLuint &location, const unsigned int &v)
    {
        glUniform1ui(location, v);
    }
    static void setUniform(const GLuint &location, const bool &v)
    {
        glUniform1i(location, v);
    }
	static void setUniform(const GLuint& location, const double& v)
	{
		glUniform1d(location, v);
	}
};


GRAPHIC_NAMESPACE_END