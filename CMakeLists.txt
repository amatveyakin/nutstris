cmake_minimum_required(VERSION 2.6)
project(Nutstris)

set(executable_name nutstris)
set(src_dir ${CMAKE_SOURCE_DIR}/src)
set(srcs
    ${src_dir}/main.cpp
    ${src_dir}/engine/engine.cpp
    ${src_dir}/engine/keyboard.cpp
    ${src_dir}/render/renderer.cpp
    ${src_dir}/render/shader.cpp
    ${src_dir}/render/shaderprogram.cpp
    ${src_dir}/render/cubemesh.cpp
    ${src_dir}/render/textureloader.cpp
    ${src_dir}/render/matrixutil.cpp
    ${src_dir}/render/playerviewport.cpp
    ${src_dir}/util/file.cpp
)

include_directories(${src_dir})

add_executable(${executable_name}  ${srcs})

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(${executable_name} ${SFML_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

add_definitions("-std=c++14 -fext-numeric-literals")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Wunused -Wformat=2 -Wcast-qual -Wold-style-cast -Woverloaded-virtual -Wshadow -Wnon-virtual-dtor -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
