#include <core/shader.hpp>

#include <fstream>
#include <iostream>

using namespace std;
using namespace RESTART;

Shader& Shader::activate()
{
    glUseProgram(m_id);
    return *this;
}

void Shader::bind(unsigned int location, float value) { glUniform1f(location, value); }

void Shader::bind(unsigned int location, glm::mat4 const& matrix) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)); }

Shader& Shader::attach(const ShaderItem& shaderItem)
{
    glAttachShader(m_id, shaderItem.get());
    return *this;
}

Shader& Shader::attach(const string& filename, ShaderItem::ShaderType type)
{
    unique_ptr<ShaderItem> item(new ShaderItem(filename, type));
    attach(*item);
    return *this;
}

Shader& Shader::link()
{
    GLint status = -1;
    GLint length = 0;

    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    if (status == -1)
    {
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

        std::unique_ptr<char[]> buffer(new char[length]);
        glGetProgramInfoLog(m_id, length, nullptr, buffer.get());

        fprintf(stderr, "%s", buffer.get());
    }

    assert(status != -1);

    return *this;
}

ShaderItem::ShaderItem(const string& filename, ShaderType type)
        : m_id(0), m_type(type), m_isValid(false)
{
    string path = PROJECT_RESOURCE_DIR"/shader/";
    ifstream fd(path + filename);
    auto src = string(istreambuf_iterator<char>(fd), istreambuf_iterator<char>());

    create(m_type) ? compile(src.c_str()) : false;
}

ShaderItem::ShaderItem(ShaderType type)
        : m_id(0), m_type(type), m_isValid(false)
{
}

bool ShaderItem::create(ShaderType type)
{
    if (type >= NB_OF_SHADER_TYPE || type < 0)
        return false;

    GLenum shaderType[NB_OF_SHADER_TYPE] =
            {GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_COMPUTE_SHADER, GL_FRAGMENT_SHADER};

    m_id = glCreateShader(shaderType[type]);

    return m_id != 0;
}

bool ShaderItem::compile(const char *code)
{
    GLint status = -1;
    GLint length = 0;

    glShaderSource(m_id, 1, &code, nullptr);
    glCompileShader(m_id);

    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    if (status == -1)
    {
        m_isValid = false;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);

        unique_ptr<char[]> buffer(new char[length]);
        glGetShaderInfoLog(m_id, length, nullptr, buffer.get());

        fprintf(stderr, "%s\n", buffer.get());
        assert(m_isValid);
    }

    m_isValid = true;
    return m_isValid;
}

bool ShaderItem::end()
{
    return create(m_type) ? compile(m_code.c_str()) : (cerr << "shader create failed\n", false);
}