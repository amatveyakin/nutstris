#ifndef TEXTUREARRAY_H
#define TEXTUREARRAY_H
#include <vector>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

namespace render {

class TextureArray {
public:
  TextureArray(int textureSlotIndex, const std::vector<sf::Image>& images);
  ~TextureArray();

  int getTextureSlotIndex() const;
  
private:
  GLuint id_;
  int textureSlotIndex_;
};

}

#endif // TEXTUREARRAY_H
