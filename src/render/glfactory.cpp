#include "glfactory.h"

namespace render {
namespace glfactory {

GLHandle genBuffer() {
  GLuint res;
  glGenBuffers(1, &res);
  return res;
}

GLHandle genVertexArray() {
  GLuint res;
  glGenVertexArrays(1, &res);
  return res;
}

GLHandle genTexture() {
  GLuint res;
  glGenTextures(1, &res);
  return res;
}

}
}

