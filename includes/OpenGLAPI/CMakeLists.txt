cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 17)

project(OpenGLAPI VERSION 1.0.1 DESCRIPTION "API to make opengl programming easier")
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_library(${PROJECT_NAME} SHARED includes/glad.c src/OpenGLAPI.cpp src/InputManager.cpp src/ShaderComponent.cpp src/SpriteRenderer.cpp)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
install(TARGETS ${PROJECT_NAME})
