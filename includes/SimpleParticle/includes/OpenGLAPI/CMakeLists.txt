cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 17)

project(OpenGLAPI VERSION 1.0.1 DESCRIPTION "API to make opengl programming easier")

SET(SOURCES includes/glad/src/glad.c includes/glad/include/glad/glad.h includes/glad/include/KHR/khrplatform.h
		src/OpenGLAPI.cpp src/InputManager.cpp
	    includes/glm/glm.hpp
	    includes/Shader.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_subdirectory("includes/glfw")

target_include_directories(${PROJECT_NAME} PRIVATE includes "includes/glad/include")

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)
install(TARGETS ${PROJECT_NAME})
