#pragma once

#include <vector>
#include <unordered_set>
#include <GL/glew.h>

#include "render/basebuffer.h"

namespace render {

template<class DataType>
class ArrayBuffer : public BaseBuffer<DataType, GL_ARRAY_BUFFER>
{
public:
  void setUpLayout();

  void setAttributeDivisor(int divisor);
  
  void setUpFloatAttribute(int attributeIndex, size_t count, bool normalized, size_t offset);
  void setUpIntAttribute(int attributeIndex, size_t count, size_t offset);
 
private:
  std::unordered_set<int> enabledAttributes_;
};

}

#include "render/arraybuffer.hpp"
