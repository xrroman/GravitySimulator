#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader;

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

struct TextureBinding
{
    GLuint id;
    std::string uniformName;
};

/// Owns a VAO/VBO/EBO triple. Non copyable GPU objects destroyed in destructor.
class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<TextureBinding> textures);

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void draw(const Shader& shader) const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<TextureBinding> m_textures;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    void setupMesh();
};
