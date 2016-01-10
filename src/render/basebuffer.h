#pragma once

#include <vector>
#include <GL/glew.h>

#include "render/glhandle.h"

namespace render {

template<class DataType, GLenum Type>
class BaseBuffer
{
public:
  BaseBuffer();
  ~BaseBuffer();

  void bind();
  void unbind();
  
  void setData(const std::vector<DataType>& data);
  
protected:
  GLHandle id_;
};

}

#include "render/basebuffer.hpp"
