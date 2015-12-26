#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <string>
#include <GL/glew.h>

namespace render {

class TextureLoader
{
public:
  TextureLoader(const std::string& path);
  ~TextureLoader();

private:
  GLuint textureId_;
};

}

#endif // TEXTURELOADER_H
