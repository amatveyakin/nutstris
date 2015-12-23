#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>

namespace render {
class Shader {
protected:
  Shader(GLenum shaderType, const std::string& filename);
  ~Shader();

public:
  GLuint getID();
  
private:
  GLuint id_ = 0;
};

  
class VertexShader : public Shader {
public:
  explicit VertexShader(const std::string& filename);
};

class FragmentShader : public Shader {
public:
  explicit FragmentShader(const std::string& filename);
};


}


#endif // SHADER_H