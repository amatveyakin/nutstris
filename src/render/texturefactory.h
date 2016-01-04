#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H
#include <memory>
#include "render/texturearray.h"

namespace render {

class TextureFactory
{
public:
  static std::unique_ptr<TextureArray> createBonusesTexture();
};

}

#endif // TEXTUREFACTORY_H
