#pragma once

#include <memory>
#include <GL/glew.h>

#include "math/vector.h"
#include "math/matrix.h"
#include "render/arraybuffer.h"
#include "render/dataformats.h"

namespace render {
class ShaderProgram;

class CubeMesh {
public:
  CubeMesh(float smoothnessRadius);
  ~CubeMesh();

  void render(const std::vector<dataformats::CubeInstance>& cubesData);

  ShaderProgram& getShaderProgram();

private:
  std::vector<dataformats::UncoloredVertex> createVerticesPositionsAndNormals_(float scale, float smoothnessRadius, int angleSteps) const;
  std::vector<GLuint> createIndices_(int angleSteps) const;

  GLHandle vertexArrayID_;

  ArrayBuffer<dataformats::UncoloredVertex> cubeVerticesBuffer_;
  ArrayBuffer<dataformats::CubeInstance> perCubeBuffer_;

  std::unique_ptr<ShaderProgram> shaderProgram_;

  size_t nIndices_;
};

}
