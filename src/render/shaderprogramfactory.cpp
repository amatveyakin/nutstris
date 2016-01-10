#include "shaderprogramfactory.h"
#include "render/shader.h"
#include "render/shaderprogram.h"
#include "render/shaders/basecubes.h"
#include "render/shaders/textured.h"

namespace render {

std::unique_ptr<ShaderProgram> ShaderProgramFactory::createBaseCubesRenderingProgram() {
  VertexShader vertexShader(shaders::cBaseCubesVertexShader);
  FragmentShader fragmentShader(shaders::cBaseCubesFragmentShader);
  return std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}

std::unique_ptr< ShaderProgram > ShaderProgramFactory::createTexturedRenderingProgram() {
  VertexShader vertexShader(shaders::cTexturedVertexShader);
  FragmentShader fragmentShader(shaders::cTexturedFragmentShader);
  return std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}


}
