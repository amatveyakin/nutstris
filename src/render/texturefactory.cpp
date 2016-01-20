#include "texturefactory.h"

#include <map>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

#include "engine/bonus.h"
#include "engine/strings.h"
#include "util/file.h"

namespace render {

namespace
{
const int kBonusesTextureSlot = 1;
const int kBackWallTextureSlot = 2;
const int kAimTextureSlot = 3;
}

std::unique_ptr<TextureArray> TextureFactory::createBonusesTexture() {
  std::vector<sf::Image> images;
  bool successfullyLoaded = true;
  for (auto bonus : engine::ForEachBonus()) {
    auto fullFileName = util::joinPath(engine::kTextureBonusesPath, getBonusName(bonus) + ".png");
    images.push_back( {});
    successfullyLoaded = images.back().loadFromFile(fullFileName) && successfullyLoaded;
  }
  assert(successfullyLoaded);
  return std::make_unique<TextureArray>(kBonusesTextureSlot, images);
}

std::unique_ptr<TextureArray> TextureFactory::createWallTexture() {
  std::vector<sf::Image> images;
  for (int i = 1;; ++i) {
    auto fullFileName = util::joinPath(engine::kTextureWallPath, std::to_string(i) + ".jpg");
    sf::Image img;
    if (img.loadFromFile(fullFileName))
      images.push_back(img);
    else
      break;
  }
  return std::make_unique<TextureArray>(kBackWallTextureSlot, images);
}

}
