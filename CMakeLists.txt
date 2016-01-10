cmake_minimum_required(VERSION 2.6)
project(Nutstris)

set(executable_name nutstris)
set(src_dir ${CMAKE_SOURCE_DIR}/src)
set(srcs
    ${src_dir}/engine/bonus.cpp
    ${src_dir}/engine/bonus.h
    ${src_dir}/engine/defs.h
    ${src_dir}/engine/field.h
    ${src_dir}/engine/game.cpp
    ${src_dir}/engine/game.h
    ${src_dir}/engine/input/control.cpp
    ${src_dir}/engine/input/control.h
    ${src_dir}/engine/input/keyboard.cpp
    ${src_dir}/engine/input/keyboard.h
    ${src_dir}/engine/piece.cpp
    ${src_dir}/engine/piece.h
    ${src_dir}/engine/player.cpp
    ${src_dir}/engine/player.h
    ${src_dir}/engine/strings.h
    ${src_dir}/engine/units.cpp
    ${src_dir}/engine/units.h
    ${src_dir}/engine/visual.h
    ${src_dir}/engine/visual/effect.cpp
    ${src_dir}/engine/visual/effect.h
    ${src_dir}/engine/visual/misc.h
    ${src_dir}/engine/visual/motion.cpp
    ${src_dir}/engine/visual/motion.h
    ${src_dir}/engine/visual/object.h
    ${src_dir}/main.cpp
    ${src_dir}/math/basic.h
    ${src_dir}/math/easing.cpp
    ${src_dir}/math/easing.h
    ${src_dir}/math/matrix.h
    ${src_dir}/math/metric.h
    ${src_dir}/math/vector.h
    ${src_dir}/render/arraybuffer.h
    ${src_dir}/render/arraybuffer.hpp
    ${src_dir}/render/basebuffer.h
    ${src_dir}/render/basebuffer.hpp
    ${src_dir}/render/cubemesh.cpp
    ${src_dir}/render/cubemesh.h
    ${src_dir}/render/dataformats.cpp
    ${src_dir}/render/dataformats.h
    ${src_dir}/render/defs.h
    ${src_dir}/render/glhandle.h
    ${src_dir}/render/glfactory.cpp
    ${src_dir}/render/glfactory.h
    ${src_dir}/render/matrixutil.cpp
    ${src_dir}/render/matrixutil.h
    ${src_dir}/render/playerviewport.cpp
    ${src_dir}/render/playerviewport.h
    ${src_dir}/render/renderer.cpp
    ${src_dir}/render/renderer.h
    ${src_dir}/render/shader.cpp
    ${src_dir}/render/shader.h
    ${src_dir}/render/shaderprogram.cpp
    ${src_dir}/render/shaderprogram.h
    ${src_dir}/render/shaderprogramfactory.cpp
    ${src_dir}/render/shaderprogramfactory.h
    ${src_dir}/render/shaders/basecubes.h
    ${src_dir}/render/shaders/textured.h
    ${src_dir}/render/shaders/lighting.h
    ${src_dir}/render/texturearray.cpp
    ${src_dir}/render/texturearray.h
    ${src_dir}/render/texturedquad.cpp
    ${src_dir}/render/texturedquad.h
    ${src_dir}/render/texturefactory.cpp
    ${src_dir}/render/texturefactory.h
    ${src_dir}/render/uniformbuffer.h
    ${src_dir}/util/containers.h
    ${src_dir}/util/file.cpp
    ${src_dir}/util/file.h
    ${src_dir}/util/macro.h
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

if(WIN32)
else()
add_definitions("-std=c++14 -fext-numeric-literals")
endif()

if(WIN32)
  file(GLOB SFMLDlls "${SFML_ROOT}/bin/*.dll")
  file(COPY ${SFMLDlls} DESTINATION "${CMAKE_SOURCE_DIR}/vsproj/Debug/")
  file(COPY ${SFMLDlls} DESTINATION "${CMAKE_SOURCE_DIR}/vsproj/Release/")

  set(GlewDll "${GLEW_INCLUDE_DIRS}/../bin/Release/x64/glew32.dll")
  file(COPY ${GlewDll} DESTINATION "${CMAKE_SOURCE_DIR}/vsproj/Debug/")
  file(COPY ${GlewDll} DESTINATION "${CMAKE_SOURCE_DIR}/vsproj/Release/")
endif()

if(WIN32)
  set(CMAKE_CXX_FLAGS "/EHsc /W4 /wd4267 /wd4244 /wd4268 /wd4592 /WX")
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Wunused -Wformat=2 -Wcast-qual -Wold-style-cast -Woverloaded-virtual -Wshadow -Wnon-virtual-dtor -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()
