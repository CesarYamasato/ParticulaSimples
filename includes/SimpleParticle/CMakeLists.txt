cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 17)

project(SimpleParticle VERSION 1.0.1 DESCRIPTION "Library that allows for the creation of simple particles")
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_library(${PROJECT_NAME} SHARED Particle.cpp)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
install(TARGETS ${PROJECT_NAME})
