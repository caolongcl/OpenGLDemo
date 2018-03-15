#include <core/texture.hpp>
#include <iostream>

using namespace std;
using namespace RESTART;

Texture::Texture()
{
    glGenTextures(1, &m_id);
}

Texture::Texture(int width, int height)
        : m_width(width), m_height(height)
{
    glGenTextures(1, &m_id);
}

Texture::Texture(const string& filename)
        : m_width(0), m_height(0)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    int nrChannels;
    string path = PROJECT_RESOURCE_DIR"/texture/" + filename;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &m_width, &m_height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        cerr << "Failed to load texture\n";
    }

    stbi_image_free(data);
}

Texture& Texture::load(const std::string& filename)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    int nrChannels;
    string path = PROJECT_RESOURCE_DIR"/texture/" + filename;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &m_width, &m_height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        cerr << "Failed to load texture\n";
    }

    stbi_image_free(data);
}

Texture& Texture::setProperty(GLenum property, GLenum value)
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, property, value);
}

Texture& Texture::setDefaultProperty()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture& Texture::genMipmap()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}
