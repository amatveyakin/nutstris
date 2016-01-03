#include "textureloader.h"
#include <map>
#include <string>
#include <cassert>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

namespace render {

namespace
{
const std::map<int, std::string> kTextureNames = {
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
}

TextureLoader::TextureLoader(const std::string& path)
{
    std::vector<sf::Image> images;
    for (auto& textureName : kTextureNames) {
        auto fullFileName = path + textureName.second + ".png";
        images.push_back( {});
        assert(images.back().loadFromFile(fullFileName));
    }

    auto width = images.front().getSize().x;
    auto height = images.front().getSize().y;

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1,&textureId_);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId_);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8,
                   width, height, images.size());

    for (size_t i = 0; i < images.size(); ++i)
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, images[i].getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

TextureLoader::~TextureLoader()
{
    glDeleteTextures(1, &textureId_);
}

}