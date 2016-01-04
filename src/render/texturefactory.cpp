#include "texturefactory.h"
#include <map>
#include <string>
#include <cassert>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

namespace render {

namespace
{
const std::string kTextureBonusesPath = "resources/textures/bonuses/";
const std::map<int, std::string> kTextureBonusesNames = {
  {0, "NoBonus"},
  {1, "EnlargeHintQueue"},
  {2, "PieceTheft"},
  {3, "Heal"},
  {4, "SlowDown"},
  {5, "ClearField"},
  {6, "FlippedScreen"},
  {7, "RotatingScreen"},
  {8, "Wave"},
  {9, "Lantern"},
  {10, "CrazyPieces"},
  {11, "TruncatedBlocks"},
  {12, "NoHint"},
  {13, "SpeedUp"}
};

const int kBonusesTextureSlot = 1;
const int kBackWallTextureSlot = 2;
const int kAimTextureSlot = 3;
}

std::unique_ptr<TextureArray> render::TextureFactory::createBonusesTexture() {
    std::vector<sf::Image> images;
    for (auto& textureName : kTextureBonusesNames) {
        auto fullFileName = kTextureBonusesPath + textureName.second + ".png";
        images.push_back( {});
        assert(images.back().loadFromFile(fullFileName));
    }
    return std::make_unique<TextureArray>(kBonusesTextureSlot, images);
}

}