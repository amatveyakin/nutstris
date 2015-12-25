#ifndef CUBEMESH_H
#define CUBEMESH_H
#include <memory>
#include <GL/glew.h>
#include "math/vector.h"
#include "math/matrix.h"

namespace render {
class ShaderProgram;

class CubeMesh {
public:
  CubeMesh();
  ~CubeMesh();

  struct PerCubeData {
    math::Mat4x4f modelToWorld;
    math::Vec3f   color;
  };

  void render(const std::vector<PerCubeData>& cubesData);

private:
  std::vector<std::pair<math::Vec3f, math::Vec3f>> createVerticesPositionsAndNormals_(float scale, float smoothnessRadius, int angleSteps) const;
  std::vector<GLuint> createIndices_(int angleSteps) const;

  GLuint vertexArrayID_;
  GLuint transformsBuffer_;
  std::unique_ptr<ShaderProgram> shaderProgram_;

  size_t nIndices_;
};

}
#endif // CUBEMESH_H
