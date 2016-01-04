#include "cubemesh.h"

#include "math/basic.h"
#include "render/shader.h"
#include "render/shaderprogram.h"
#include "render/defs.h"
#include "render/shaderprogramfactory.h"

namespace render {

namespace {
const double kCubeSmoothRadius = 0.4f;
const int    kCubeAngleSteps   = 10;
}

void CubeMesh::PerCubeData::setUpLayout(ArrayBuffer<PerCubeData>& buffer) {
  const int textureIDLayout = 2;
  const int transformLayout = 4;
  const int colorLayout = 8;

  for (int i = 0; i < 4; i++)
    buffer.setUpFloatAttribute(transformLayout + i, 4, false, sizeof(float) * 4 * i);

  buffer.setUpFloatAttribute(colorLayout, 3, false, offsetof(PerCubeData, color));
  buffer.setUpIntAttribute(textureIDLayout, 1, offsetof(PerCubeData, textureIndex));
}

CubeMesh::CubeMesh() {
  glGenVertexArrays(1, &vertexArrayID_);
  glBindVertexArray(vertexArrayID_);

  cubeVerticesBuffer_.bind();
  cubeVerticesBuffer_.setUpLayout();
  cubeVerticesBuffer_.unbind();
  cubeVerticesBuffer_.setData(createVerticesPositionsAndNormals_(1.0 / 2.0f, kCubeSmoothRadius, kCubeAngleSteps));

  perCubeBuffer_.bind();
  perCubeBuffer_.setUpLayout();
  perCubeBuffer_.setAttributeDivisor(1);
  perCubeBuffer_.unbind();

  GLuint indexBuffer;
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  auto indicesData = createIndices_(kCubeAngleSteps);
  nIndices_ = indicesData.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData[0]) * indicesData.size(), indicesData.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  shaderProgram_ = ShaderProgramFactory::createBaseCubesRenderingProgram();
}

CubeMesh::~CubeMesh() {

}

void CubeMesh::render(const std::vector<PerCubeData>& cubesData) {
  perCubeBuffer_.setData(cubesData);
  shaderProgram_->makeActive();
  glBindVertexArray(vertexArrayID_);
  glDrawElementsInstanced(GL_TRIANGLES, nIndices_, GL_UNSIGNED_INT, nullptr, cubesData.size() );
}

ShaderProgram& CubeMesh::getShaderProgram() {
  return *shaderProgram_;
}

std::vector<dataformats::PointWithNormal> CubeMesh::createVerticesPositionsAndNormals_(float scale, float smoothnessRadius, int angleSteps) const {
  auto nVertices = 8 * angleSteps * angleSteps;
  std::vector<dataformats::PointWithNormal> result(nVertices);

  for (auto i = 0; i < 2 * angleSteps; ++i)
  {
    for (auto j = 0; j < 4 * angleSteps; ++j)
    {
      auto alpha = ((i - angleSteps + 0.5) * (M_PI / 2)) / angleSteps;
      auto beta  = ((j + 0.5) * (M_PI / 2)) / angleSteps;

      auto x = cosf(alpha) * cosf(beta);
      auto z = cosf(alpha) * sinf(beta);
      auto y = sinf(alpha);

      result[i * 4 * angleSteps + j].point = { scale * (smoothnessRadius * x + (1 - smoothnessRadius) * math::sgn(x)),
					       scale * (smoothnessRadius * y + (1 - smoothnessRadius) * math::sgn(y)),
                                               scale * (smoothnessRadius * z + (1 - smoothnessRadius) * math::sgn(z))  };

      result[i * 4 * angleSteps + j].normal = {x, y, z};
    }
  }

  return result;
}

std::vector<GLuint> CubeMesh::createIndices_(int angleSteps) const {
  auto nVertices = 8 * angleSteps * angleSteps;
  auto nFaces = (2 * angleSteps - 1) * (8 * angleSteps + 4);
  std::vector<GLuint> result(3 * nFaces);

  for (auto i = 0; i < 2 * angleSteps - 1; ++i)
  {
    for (auto j = 0; j < 4 * angleSteps; ++j)
    {
      result[6 * (i * 4 * angleSteps + j) + 0] = i * 4 * angleSteps + j;
      result[6 * (i * 4 * angleSteps + j) + 1] = (i + 1) * 4 * angleSteps + j;
      result[6 * (i * 4 * angleSteps + j) + 2] = (i + 1) * 4 * angleSteps + ((j + 1) % (4 * angleSteps));

      result[6 * (i * 4 * angleSteps + j) + 3] = i * 4 * angleSteps + j;
      result[6 * (i * 4 * angleSteps + j) + 4] = (i + 1) * 4 * angleSteps + ((j + 1) % (4 * angleSteps));
      result[6 * (i * 4 * angleSteps + j) + 5] = i * 4 * angleSteps + ((j + 1) % (4 * angleSteps));
    }
  }

  for (auto i = 0; i < 4 * angleSteps - 2; ++i)
  {
    result[(2 * angleSteps - 1) * 24 * angleSteps + 3 * i + 0] = 0;
    result[(2 * angleSteps - 1) * 24 * angleSteps + 3 * i + 1] = i + 1;
    result[(2 * angleSteps - 1) * 24 * angleSteps + 3 * i + 2] = i + 2;
  }

  for (auto i = 0; i < 4 * angleSteps - 2; ++i)
  {
    result[nFaces * 3 - 3 * i - 1] = nVertices - 1;
    result[nFaces * 3 - 3 * i - 3] = nVertices - 2 - i;
    result[nFaces * 3 - 3 * i - 2] = nVertices - 3 - i;
  }

  return result;
}




}