#include "Trail.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

Trail::Trail()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(kMaxPoints * sizeof(glm::vec3)),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Trail::~Trail()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Trail::addPoint(const glm::vec3& position)
{
    m_points.push_back(position);
    if (m_points.size() > kMaxPoints)
        m_points.pop_front();
}

void Trail::upload() const
{
    const std::vector<glm::vec3> pts(m_points.begin(), m_points.end());
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    static_cast<GLsizeiptr>(pts.size() * sizeof(glm::vec3)),
                    pts.data());
}

void Trail::draw(const glm::vec3& color,
                 const Shader& shader,
                 const glm::mat4& view,
                 const glm::mat4& projection) const
{
    if (m_points.size() < 2) return;

    upload();

    shader.bind();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setVec3("uColor", color);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_points.size()));
    glBindVertexArray(0);
}
