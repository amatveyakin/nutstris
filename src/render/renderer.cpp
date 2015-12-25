#include "render/renderer.h"
#include "render/cubemesh.h"

namespace render {

Renderer::Renderer() {
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  cubeMesh_ = std::make_unique<CubeMesh>();
}

Renderer::~Renderer() {

}

void Renderer::render() {
  std::vector<CubeMesh::PerCubeData> cubesData;
  for (int i = -5; i <= 5; ++i)
    for (int j = -5; j <= 5; ++j)
      cubesData.push_back({
	math::Mat4x4f::translationMatrix({i * 1.0f, j * 1.0f, 0.0f}),
	math::Vec3f(0.5 + i * 0.1f, 0.5 + j * 0.1f, 0.0f)
      });
  cubeMesh_->render(cubesData);
}

}