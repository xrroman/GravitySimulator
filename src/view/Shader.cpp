#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    const std::string vertSource = loadFile(vertexPath);
    const std::string fragSource = loadFile(fragmentPath);

    const GLuint vert = compileShader(GL_VERTEX_SHADER, vertSource);
    const GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSource);

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vert);
    glAttachShader(m_programID, frag);
    glLinkProgram(m_programID);

    GLint success = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(m_programID, sizeof(log), nullptr, log);
        std::cerr << "[Shader] Link error:\n" << log << '\n';
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader()
{
    if (m_programID != 0)
        glDeleteProgram(m_programID);
}

void Shader::bind() const { glUseProgram(m_programID); }
void Shader::unbind() const { glUseProgram(0); }

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v) const
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_programID, name.c_str());
}

GLuint Shader::compileShader(GLenum type, const std::string& source)
{
    const GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(id, sizeof(log), nullptr, log);
        const char* typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cerr << "[Shader] " << typeName << " compile error:\n" << log << '\n';
    }
    return id;
}

std::string Shader::loadFile(const char* path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::ostringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cerr << "[Shader] Could not read file '" << path << "': " << e.what() << '\n';
        return {};
    }
}
