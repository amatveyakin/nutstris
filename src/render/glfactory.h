#pragma once

#include <GL/glew.h>
#include "render/glhandle.h"

namespace render {
namespace glfactory {

GLHandle genBuffer();

GLHandle genVertexArray();

GLHandle genTexture();

}
}

