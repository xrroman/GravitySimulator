#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <deque>
#include <vector>

class Shader;

/// Stores and renders the orbital trail of a single body.
/// Points are kept in a ring-buffer capped at kMaxPoints.
/// The GPU buffer is pre-allocated once and updated via glBufferSubData.
class Trail
{
public:
    static constexpr std::size_t kMaxPoints = 512;

    Trail();
    ~Trail();

    Trail(const Trail&) = delete;
    Trail& operator=(const Trail&) = delete;
    Trail(Trail&&) = default;
    Trail& operator=(Trail&&) = default;

    /// Appends a world space position; evicts the oldest point when full.
    void addPoint(const glm::vec3& position);

    void draw(const glm::vec3& color,
              const Shader& shader,
              const glm::mat4& view,
              const glm::mat4& projection) const;

private:
    std::deque<glm::vec3> m_points;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    void upload() const;
};
