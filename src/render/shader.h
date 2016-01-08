#pragma once

#include <string>
#include <GL/glew.h>

namespace render {
class Shader {
protected:
  Shader(GLenum shaderType, const std::string& sourceCode);
  ~Shader();

public:
  GLuint getID();
  
private:
  GLuint id_ = 0;
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
