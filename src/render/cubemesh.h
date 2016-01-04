#ifndef CUBEMESH_H
#define CUBEMESH_H
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
  CubeMesh();
  ~CubeMesh();

  void render(const std::vector<dataformats::CubeInstance>& cubesData);

  ShaderProgram& getShaderProgram();

private:
  std::vector<dataformats::UncoloredVertex> createVerticesPositionsAndNormals_(float scale, float smoothnessRadius, int angleSteps) const;
  std::vector<GLuint> createIndices_(int angleSteps) const;

  GLuint vertexArrayID_;

  ArrayBuffer<dataformats::UncoloredVertex> cubeVerticesBuffer_;
  ArrayBuffer<dataformats::CubeInstance> perCubeBuffer_;

  std::unique_ptr<ShaderProgram> shaderProgram_;

  size_t nIndices_;
};

}
#endif // CUBEMESH_H
