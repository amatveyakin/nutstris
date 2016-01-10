#include "texturedquad.h"
#include "render/shaderprogramfactory.h"
#include "render/glfactory.h"

namespace render {

TexturedQuad::TexturedQuad ( float worldWidth, float worldHeight, float texWidth, float texHeight ) 
  : vertexArrayID_(glfactory::genVertexArray())
{
  glBindVertexArray(vertexArrayID_);
  verticesBuffer_.bind();
  verticesBuffer_.setUpLayout();
  verticesBuffer_.unbind();
  
  std::vector<dataformats::TexturedVertex> vertices =
  {
    {{ worldWidth / 2.0f, -worldHeight / 2.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {texWidth, texHeight}},
    {{-worldWidth / 2.0f, -worldHeight / 2.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, texHeight}},
    {{ worldWidth / 2.0f,  worldHeight / 2.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {texWidth, 0.0f}},
    {{-worldWidth / 2.0f,  worldHeight / 2.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}
  };
  verticesBuffer_.setData(vertices);
  
  glBindVertexArray(0);
  
  shaderProgram_ = ShaderProgramFactory::createTexturedRenderingProgram();
}

void TexturedQuad::render() {
  shaderProgram_->makeActive();
  glBindVertexArray(vertexArrayID_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

ShaderProgram& TexturedQuad::getShaderProgram() {
  return *shaderProgram_;
}


}