cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 17)

project(ParticleAPI VERSION 1.0.1 DESCRIPTION "API to make opengl programming easier")
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_library(ParticleAPI SHARED Particle.cpp)
target_link_libraries(ParticleAPI glfw OpenGL::GL)
install(TARGETS ParticleAPI)
