#pragma once

#include <vector>
#include <unordered_set>
#include <GL/glew.h>

#include "render/glhandle.h"

namespace render {

template<class DataType>
class ArrayBuffer
{
public:
  ArrayBuffer();
  ~ArrayBuffer();
  
  void bind();
  void unbind();
  
  void setUpLayout();
  void setData(const std::vector<DataType>& data);
  
  void setAttributeDivisor(int divisor);
  
  void setUpFloatAttribute(int attributeIndex, size_t count, bool normalized, size_t offset);
  void setUpIntAttribute(int attributeIndex, size_t count, size_t offset);
 
private:
  GLHandle id_;
  std::unordered_set<int> enabledAttributes_;
};

}

#include "render/arraybuffer.hpp"
