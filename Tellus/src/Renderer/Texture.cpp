#include "Texture.hpp"
#include "tspch.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace ts {

Texture2D::Texture2D(const std::string& path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    TS_ASSERT(data, "Failed to load image!");
    m_Width = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
    glTextureStorage2D(m_ID, 1, GL_RGB8, m_Width, m_Height);

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}
Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_ID);
}

void Texture2D::Bind(unsigned int slot) const {
    glBindTextureUnit(slot, m_ID);
}
void Texture2D::Unbind(unsigned int slot) const {
    glBindTextureUnit(slot, 0);
}

};  // namespace ts