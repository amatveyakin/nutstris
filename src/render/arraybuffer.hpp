namespace render {

namespace {
GLuint genBuffer() {
  GLuint res;
  glGenBuffers(1, &res);
  return res;
}
}


template<class DataType>
ArrayBuffer<DataType>::ArrayBuffer() 
  : id_(genBuffer())
{}

template<class DataType>
ArrayBuffer<DataType>::~ArrayBuffer() {
  glDeleteBuffers(1, &id_.get());
}

template<class DataType>
void ArrayBuffer<DataType>::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, id_);
}

template<class DataType>
void ArrayBuffer<DataType>::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

template<class DataType>
void ArrayBuffer<DataType>::setData(const std::vector<DataType>& data) {
  bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(DataType) * data.size(), data.data(), GL_STATIC_DRAW);
  unbind();
}

}