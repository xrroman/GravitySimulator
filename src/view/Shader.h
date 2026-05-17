#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

/// Compiles and links a GLSL vertex + fragment shader pair.
/// Wraps the GL program object and provides typed uniform setters.
class Shader
{
public:
    /// Prints compilation/link errors to stderr; program ID will be 0 on failure.
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    void bind() const;
    void unbind() const;

    void setInt(const std::string& name, int value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& v) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    [[nodiscard]] GLuint getProgramID() const { return m_programID; }

private:
    GLuint m_programID = 0;

    [[nodiscard]] static GLuint compileShader(GLenum type, const std::string& source);
    [[nodiscard]] static std::string loadFile(const char* path);
    [[nodiscard]] GLint getUniformLocation(const std::string& name) const;
};
