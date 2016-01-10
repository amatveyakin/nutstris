#pragma once

#include <vector>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

#include "render/glhandle.h"

namespace render {

class TextureArray {
public:
  TextureArray(int textureSlotIndex, const std::vector<sf::Image>& images);
  ~TextureArray();

  int getTextureSlotIndex() const;
  size_t getTextureCount() const;
  
private:
  GLHandle id_;
  int textureSlotIndex_;
  size_t textureCount_;
};

}
