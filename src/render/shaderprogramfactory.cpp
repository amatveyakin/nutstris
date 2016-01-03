#include "shaderprogramfactory.h"
#include "render/shader.h"
#include "render/shaderprogram.h"
#include "render/shaders/basecubes.h"

namespace render {

std::unique_ptr<ShaderProgram> ShaderProgramFactory::createBaseCubesRenderingProgram() {
  auto vertexShader = VertexShader(shaders::cBaseCubesVertexShader);
  auto fragmentShader = FragmentShader(shaders::cBaseCubesFragmentShader);
  return std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}

}
