cmake_minimum_required(VERSION 2.6)
project(Nutstris)

set(executable_name nutstris)
set(src_dir ${CMAKE_SOURCE_DIR}/src)
set(srcs
    ${src_dir}/main.cpp
    ${src_dir}/engine/engine.cpp
    ${src_dir}/engine/keyboard.cpp
    ${src_dir}/engine/visual/effect.cpp
    ${src_dir}/engine/visual/motion.cpp
    ${src_dir}/math/easing.cpp
    ${src_dir}/render/renderer.cpp
    ${src_dir}/render/shader.cpp
    ${src_dir}/render/shaderprogram.cpp
    ${src_dir}/render/shaderprogramfactory.cpp
    ${src_dir}/render/cubemesh.cpp
    ${src_dir}/render/texturedquad.cpp
    ${src_dir}/render/dataformats.cpp
    ${src_dir}/render/texturearray.cpp
    ${src_dir}/render/texturefactory.cpp
    ${src_dir}/render/matrixutil.cpp
    ${src_dir}/render/playerviewport.cpp
    ${src_dir}/util/file.cpp
)

include_directories(${src_dir})

add_executable(${executable_name}  ${srcs})

if(WIN32)
  set(CMAKE_PREFIX_PATH "$ENV{programfiles}/glew" ${CMAKE_PREFIX_PATH})
  set(CMAKE_LIBRARY_PATH "$ENV{programfiles}/glew/lib/Release/x64" ${CMAKE_LIBRARY_PATH})
  set(SFML_ROOT "$ENV{ProgramW6432}/SFML")
  set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH})
else()
  set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
endif()

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

if(WIN32)
  include_directories(${GLEW_INCLUDE_DIRS})
  include_directories(${SFML_INCLUDE_DIR})
else()
endif()

target_link_libraries(${executable_name} ${SFML_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

add_definitions("-std=c++14 -fext-numeric-literals")

if(WIN32)
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Wunused -Wformat=2 -Wcast-qual -Wold-style-cast -Woverloaded-virtual -Wshadow -Wnon-virtual-dtor -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()
