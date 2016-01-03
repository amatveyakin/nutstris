#include "shaderprogramfactory.h"
#include "render/shader.h"
#include "render/shaderprogram.h"

namespace render {

std::unique_ptr<ShaderProgram> ShaderProgramFactory::createBaseCubesRenderingProgram() {
  auto vertexShader = VertexShader("src/render/shaders/dummy.vs");
  auto fragmentShader = FragmentShader("src/render/shaders/dummy.fs");
  return std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}

}
