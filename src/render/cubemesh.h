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

  struct PerCubeData {
    math::Mat4x4f modelToWorld;
    math::Vec3f   color;
    int           textureIndex;

    static void setUpLayout(ArrayBuffer<PerCubeData>& buffer);
  };

  void render(const std::vector<PerCubeData>& cubesData);

  ShaderProgram& getShaderProgram();

private:
  std::vector<dataformats::PointWithNormal> createVerticesPositionsAndNormals_(float scale, float smoothnessRadius, int angleSteps) const;
  std::vector<GLuint> createIndices_(int angleSteps) const;

  GLuint vertexArrayID_;

  ArrayBuffer<dataformats::PointWithNormal> cubeVerticesBuffer_;
  ArrayBuffer<PerCubeData> transformsBuffer_;

  std::unique_ptr<ShaderProgram> shaderProgram_;

  size_t nIndices_;
};

}
#endif // CUBEMESH_H
