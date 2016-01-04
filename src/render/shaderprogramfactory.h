#ifndef SHADERPROGRAMFACTORY_H
#define SHADERPROGRAMFACTORY_H
#include <memory>

namespace render {
class ShaderProgram;

 
class ShaderProgramFactory
{
public:
  static std::unique_ptr<ShaderProgram> createBaseCubesRenderingProgram();
  static std::unique_ptr<ShaderProgram> createTexturedRenderingProgram();
};

}

#endif // SHADERPROGRAMFACTORY_H
