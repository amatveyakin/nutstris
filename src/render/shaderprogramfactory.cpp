#include "shaderprogramfactory.h"
#include "render/shader.h"
#include "render/shaderprogram.h"
#include "render/shaders/basecubes.h"
#include "render/shaders/textured.h"
#include "shaders/lighting.h"

namespace render {

namespace {
const std::string kVersionPragma = "#version 330 core\n";
}

std::unique_ptr<ShaderProgram> ShaderProgramFactory::createBaseCubesRenderingProgram() {
  VertexShader vertexShader(kVersionPragma + shaders::kBaseCubesVertexShader);
  FragmentShader fragmentShader(kVersionPragma + shaders::kLightingHelper + shaders::kBaseCubesFragmentShader);
  return std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}

std::unique_ptr< ShaderProgram > ShaderProgramFactory::createTexturedRenderingProgram() {
  VertexShader vertexShader(kVersionPragma + shaders::kTexturedVertexShader);
  FragmentShader fragmentShader(kVersionPragma + shaders::kLightingHelper + shaders::kTexturedFragmentShader);
  return std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}


}
