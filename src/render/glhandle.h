#pragma once

#include <GL/glew.h>

namespace render {

class GLHandle
{
public:
    GLHandle(GLuint id) : id_(id) {}
    GLHandle(GLHandle&& other) : id_(other.id_) { other.id_ = GL_INVALID_INDEX; }
    GLHandle(const GLHandle& other) = delete;
    const GLHandle& operator=(const GLHandle& other) = delete;

    operator GLuint() { return id_; }
    const GLuint& get() const { return id_; }

private:
    GLuint id_;
};

}
