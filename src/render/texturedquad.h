#pragma once

#include <memory>

#include <GL/glew.h>

#include "render/arraybuffer.h"
#include "render/dataformats.h"
#include "render/shaderprogram.h"

namespace render {

class TexturedQuad {
public:
  TexturedQuad(float worldWidth, float worldHeight, float texWidth, float texHeight);
  
  void render();
  
  ShaderProgram& getShaderProgram();
  
private:
  std::unique_ptr<ShaderProgram> shaderProgram_; 
  GLHandle vertexArrayID_;
  ArrayBuffer<dataformats::TexturedVertex> verticesBuffer_;
};

}
