#pragma once

#include <glad/glad.h>

/// Loads an image from disk and uploads it as a 2D OpenGL texture.
/// RAII: GL texture object is deleted in the destructor.
class TextureLoader
{
public:
    explicit TextureLoader(const char* path);
    ~TextureLoader();

    TextureLoader(const TextureLoader&) = delete;
    TextureLoader& operator=(const TextureLoader&) = delete;
    TextureLoader(TextureLoader&&) = default;
    TextureLoader& operator=(TextureLoader&&) = default;

    [[nodiscard]] GLuint getID() const { return m_id; }
    [[nodiscard]] int getWidth() const { return m_width; }
    [[nodiscard]] int getHeight() const { return m_height; }

private:
    GLuint m_id = 0;
    int m_width = 0;
    int m_height = 0;
};
