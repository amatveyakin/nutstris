#pragma once

#include <string>
#include <GL/glew.h>

#include "render/glhandle.h"

namespace render {
class Shader {
protected:
  Shader(GLenum shaderType, const std::string& sourceCode);
  ~Shader();

public:
  GLuint getID();
  
private:
  GLHandle id_;
};

  
class VertexShader : public Shader {
public:
  explicit VertexShader(const std::string& sourceCode);
};

class FragmentShader : public Shader {
public:
  explicit FragmentShader(const std::string& sourceCode);
};


}
