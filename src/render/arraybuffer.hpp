namespace render {

template<class DataType>
void ArrayBuffer<DataType>::setUpLayout() {
  DataType::setUpLayout(*this);
}

template<class DataType>
void ArrayBuffer<DataType>::setAttributeDivisor(int divisor) {
  for (auto attributeIndex : enabledAttributes_)
    glVertexAttribDivisor(attributeIndex, divisor);
}

template<class DataType>
void ArrayBuffer<DataType>::setUpFloatAttribute(int attributeIndex, size_t count, bool normalized, size_t offset) {
  auto normalizedGL = normalized ? GL_TRUE : GL_FALSE;
  glVertexAttribPointer(attributeIndex, count, GL_FLOAT, normalizedGL, sizeof(DataType), reinterpret_cast<GLvoid*>(offset));
  glEnableVertexAttribArray(attributeIndex);
  enabledAttributes_.insert(attributeIndex);
}

template<class DataType>
void ArrayBuffer<DataType>::setUpIntAttribute(int attributeIndex, size_t count, size_t offset) {
  glVertexAttribIPointer(attributeIndex, count, GL_INT, sizeof(DataType), reinterpret_cast<GLvoid*>(offset));
  glEnableVertexAttribArray(attributeIndex);
  enabledAttributes_.insert(attributeIndex);
}

}