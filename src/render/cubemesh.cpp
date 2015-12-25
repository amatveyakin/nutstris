#include "cubemesh.h"

#include "math/basic.h"
#include "render/shader.h"
#include "render/shaderprogram.h"

namespace render {
  
CubeMesh::CubeMesh() {
  glGenVertexArrays(1, &vertexArrayID_);
  glBindVertexArray(vertexArrayID_);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  auto verticesData = createVerticesPositionsAndNormals_(0.5f, 0.1f, 10);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData[0]) * verticesData.size(), verticesData.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint indexBuffer;
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  auto indicesData = createIndices_(10);
  nIndices_ = indicesData.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData[0]) * indicesData.size(), indicesData.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(std::pair<math::Vec3f, math::Vec3f>), nullptr);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(std::pair<math::Vec3f, math::Vec3f>), reinterpret_cast<GLvoid*>(12));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  auto vertexShader = VertexShader("src/render/shaders/dummy.vs");
  auto fragmentShader = FragmentShader("src/render/shaders/dummy.fs");
  shaderProgram_ = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}

CubeMesh::~CubeMesh() {

}

void CubeMesh::render() {
  shaderProgram_->makeActive();
  glBindVertexArray(vertexArrayID_);
  glDrawElements(GL_TRIANGLES, nIndices_, GL_UNSIGNED_INT, nullptr );
}

std::vector<std::pair<math::Vec3f, math::Vec3f>> CubeMesh::createVerticesPositionsAndNormals_(float scale, float smoothnessRadius, int angleSteps) const {
  auto nVertices = 8 * angleSteps * angleSteps;
  std::vector<std::pair<math::Vec3f, math::Vec3f>> result(nVertices);

  for (auto i = 0; i < 2 * angleSteps; ++i)
  {
    for (auto j = 0; j < 4 * angleSteps; ++j)
    {
      auto alpha = ((i - angleSteps + 0.5) * (M_PI / 2)) / angleSteps;
      auto beta  = ((j + 0.5) * (M_PI / 2)) / angleSteps;

      auto x = cosf(alpha) * cosf(beta);
      auto z = cosf(alpha) * sinf(beta);
      auto y = sinf(alpha);

      result[i * 4 * angleSteps + j].first = { scale * (smoothnessRadius * x + (1 - smoothnessRadius) * math::sgn(x)),
					       scale * (smoothnessRadius * y + (1 - smoothnessRadius) * math::sgn(y)),
                                               scale * (smoothnessRadius * z + (1 - smoothnessRadius) * math::sgn(z))  };

      result[i * 4 * angleSteps + j].second = {x, y, z};
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