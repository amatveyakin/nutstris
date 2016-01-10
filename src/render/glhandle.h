#pragma once

#include <GL/glew.h>

namespace render {

class GLHandle
{
public:
    GLHandle(GLuint id) : id_(id) {}
    GLHandle(const GLHandle& other) = delete;
    const GLHandle& operator=(const GLHandle& other) = delete;

    operator GLuint() { return id_; }

private:
    const GLuint id_;
};

}
