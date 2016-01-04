#include "texturearray.h"

namespace render {

TextureArray::TextureArray ( int textureSlotIndex, const std::vector<sf::Image>& images ) 
: textureSlotIndex_(textureSlotIndex)
{
    auto width = images.front().getSize().x;
    auto height = images.front().getSize().y;

    glActiveTexture(GL_TEXTURE0 + textureSlotIndex);

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, images.size());

    for (size_t i = 0; i < images.size(); ++i)
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, images[i].getPixelsPtr());
}

TextureArray::~TextureArray() {
    glDeleteTextures(1, &id_);
}

int TextureArray::getTextureSlotIndex() const {
  return textureSlotIndex_;
}


}