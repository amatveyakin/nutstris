#pragma once

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
