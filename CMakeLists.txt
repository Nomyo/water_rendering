cmake_minimum_required(VERSION 3.5)
project(water_rendering)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -g -O0 -Wall -Wextra -Iinc")

set(

  SOURCE_FILES

  src/main.cc
  src/glad.c

  src/camera.cc
  src/glob_conf.cc
  src/opengl-utils.cc
  src/shader_m.cc
  src/stb_image.cc
  src/mesh.cc
  src/world.cc
  src/world-renderer.cc
  src/light.cc
  src/light-renderer.cc
  )


add_executable(world ${SOURCE_FILES})
target_link_libraries(world glfw3)
target_link_libraries(world GL)
target_link_libraries(world dl)
target_link_libraries(world Xinerama)
target_link_libraries(world Xrandr)
target_link_libraries(world Xi)
target_link_libraries(world Xcursor)
target_link_libraries(world X11)
target_link_libraries(world Xxf86vm)
target_link_libraries(world pthread)
target_link_libraries(world SDL2)
target_link_libraries(world SDL2main)
target_link_libraries(world SDL2_image)
