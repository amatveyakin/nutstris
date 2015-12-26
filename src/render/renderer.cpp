#include "render/renderer.h"
#include "render/cubemesh.h"
#include "render/textureloader.h"
#include "render/matrixutil.h"
#include "render/shaderprogram.h"

namespace render {

Renderer::Renderer() {
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  cubeMesh_ = std::make_unique<CubeMesh>();
  textureLoader_ = std::make_unique<TextureLoader>("resources/textures/bonuses/");
}

Renderer::~Renderer() {

}

void Renderer::render() {
  std::vector<CubeMesh::PerCubeData> cubesData;
  for (int i = -5; i <= 5; ++i)
    for (int j = -5; j <= 5; ++j)
      cubesData.push_back({
        math::Mat4x4f::translationMatrix({i * 1.0f, j * 1.0f, 0.0f}),
        math::Vec3f(0.5 + i * 0.1f, 0.5 + j * 0.1f, 0.0f),
        (i + j + 10) % 2
      });

  auto matrixView = matrixutil::lookAt({0.0, 1.5, 10.5}, {0, 0, 0}, {0, 1, 0});
  auto matrixProj = matrixutil::perspective(M_PI / 2.5f, 4.0 / 3.0, 0.01f, 100.0f);
  auto VP = matrixProj * matrixView;
  cubeMesh_->getShaderProgram().setUniform("VP", VP);
  cubeMesh_->getShaderProgram().setUniform("textures", 0);
  cubeMesh_->render(cubesData);
}

}