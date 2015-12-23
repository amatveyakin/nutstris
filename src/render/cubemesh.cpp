#include "cubemesh.h"

#include "render/shader.h"
#include "render/shaderprogram.h"

namespace render {
  
namespace {
static const GLfloat g_vertex_buffer_data[] = {
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  0.0f,  1.0f, 0.0f,
};
}

CubeMesh::CubeMesh() {
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vertexArrayID_);
  glBindVertexArray(vertexArrayID_);

  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    nullptr             // array buffer offset
  );
  glEnableVertexAttribArray(0);

  auto vertexShader = VertexShader("src/render/shaders/dummy.vs");
  auto fragmentShader = FragmentShader("src/render/shaders/dummy.fs");
  shaderProgram_ = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
}

CubeMesh::~CubeMesh() {

}

void CubeMesh::render() {
  shaderProgram_->makeActive();
  glBindVertexArray(vertexArrayID_);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


}