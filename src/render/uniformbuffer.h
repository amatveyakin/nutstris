#pragma once

#include <vector>
#include <unordered_set>
#include <GL/glew.h>

#include "render/basebuffer.h"

namespace render {

template<class DataType>
class UniformBuffer : public BaseBuffer<DataType, GL_UNIFORM_BUFFER> {
public:
  void bindTo(GLuint bindingPoint) const {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id_);
  }
};

}

