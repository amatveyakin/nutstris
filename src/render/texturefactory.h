#pragma once

#include <memory>

#include "render/texturearray.h"

namespace render {

class TextureFactory
{
public:
  static std::unique_ptr<TextureArray> createBonusesTexture();
  static std::unique_ptr<TextureArray> createWallTexture();
};

}
