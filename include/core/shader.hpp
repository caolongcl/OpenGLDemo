#pragma once

#include <base.hpp>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RESTART
{
class ShaderItem
{
public:
    using ShaderType = enum
    {
        VERTEX = 0,
        GEOMETRY,
        COMPUTE,
        FRAGMENT,
        NB_OF_SHADER_TYPE
    };

public:
    ShaderItem(const std::string& filename, ShaderType type);

    ShaderItem(ShaderType type);

    template<typename ... Args>
    ShaderType& append(const char *format, Args... args)
    {
        int length = 1024;
        char *temp = new char[length];
        snprintf(temp, length, format, args...);
        m_code += std::string(temp, strlen(temp));
        delete temp;
        return *this;
    }

    bool end();

    ~ShaderItem() { glDeleteShader(m_id); };

    const GLuint get() const { return m_id; }

private:
    ShaderItem(const ShaderItem&) = delete;

    ShaderItem& operator=(const ShaderItem&) = delete;

    bool create(ShaderType type);

    bool compile(const char *code);

    GLuint m_id;
    ShaderType m_type;
    bool m_isValid;

    std::string m_code;
};

class Shader
{
public:
    Shader() { m_id = glCreateProgram(); }

    ~Shader() { glDeleteProgram(m_id); }

    Shader& activate();

    Shader& attach(const ShaderItem& shaderCode);

    Shader& attach(const std::string& filename, ShaderItem::ShaderType type);

    GLuint get() { return m_id; }

    Shader& link();

    void bind(unsigned int location, float value);

    void bind(unsigned int location, const glm::mat4& matrix);

    //void bind(unsigned int location, const glm::vec4& vec);

    template<typename T>
    Shader& bind(const std::string& name, T&& value)
    {
        int location = glGetUniformLocation(m_id, name.c_str());
        if (location == -1) fprintf(stderr, "Missing Uniform: %s %u\n", name.c_str(), m_id);
        else bind(location, std::forward<T>(value));
        return *this;
    }

private:
    Shader(Shader const&) = delete;

    Shader& operator=(Shader const&) = delete;

    GLuint m_id;
};


};
