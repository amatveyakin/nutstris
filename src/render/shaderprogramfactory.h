#ifndef SHADERPROGRAMFACTORY_H
#define SHADERPROGRAMFACTORY_H
#include <memory>

namespace render {
class ShaderProgram;

 
class ShaderProgramFactory
{
public:
  static std::unique_ptr<ShaderProgram> createBaseCubesRenderingProgram();
};

}

#endif // SHADERPROGRAMFACTORY_H
