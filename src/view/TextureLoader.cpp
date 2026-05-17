#include "TextureLoader.h"

#include <stb/stb_image.h>
#include <iostream>

TextureLoader::TextureLoader(const char* path)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    unsigned char* data = stbi_load(path, &m_width, &m_height, &channels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     m_width, m_height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "[TextureLoader] Failed to load '" << path << "'\n";
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureLoader::~TextureLoader()
{
    if (m_id != 0)
        glDeleteTextures(1, &m_id);
}
