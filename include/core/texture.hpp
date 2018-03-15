#pragma once

#include <base.hpp>
#include <string>

namespace RESTART
{

class Texture
{
public:
    Texture();
    Texture(int width, int height);

    Texture(const std::string& filename);

    Texture& load(const std::string& filename);

    Texture& setProperty(GLenum property, GLenum value);

    Texture& setDefaultProperty();

    Texture& genMipmap();

    ~Texture() { glDeleteTextures(1, &m_id); }

private:
    GLuint m_id;
    int m_width;
    int m_height;
};

}
