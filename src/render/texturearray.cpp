#include "texturearray.h"

namespace render {

namespace {
GLuint genTexture() {
  GLuint res;
  glGenTextures(1, &res);
  return res;
}
}

TextureArray::TextureArray ( int textureSlotIndex, const std::vector<sf::Image>& images ) 
: textureSlotIndex_(textureSlotIndex),
  textureCount_(images.size()),
  id_(genTexture())
{
    auto width = images.front().getSize().x;
    auto height = images.front().getSize().y;

    glActiveTexture(GL_TEXTURE0 + textureSlotIndex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, images.size());

    for (size_t i = 0; i < images.size(); ++i)
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, images[i].getPixelsPtr());
}

TextureArray::~TextureArray() {
  GLuint id = id_;
  glDeleteTextures(1, &id);
}

int TextureArray::getTextureSlotIndex() const {
  return textureSlotIndex_;
}

size_t TextureArray::getTextureCount() const {
  return textureCount_;
}


}