cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 17)

project(SimpleParticle VERSION 1.0.1 DESCRIPTION "Library that allows for the creation of simple particles")

SET(SOURCES Particle.cpp
		includes/glad/src/glad.c includes/glad/include/glad/glad.h includes/glad/include/KHR/khrplatform.h
	    includes/glm/glm.hpp
		includes/GLFW/glfw3.h
	    includes/Shader.h
	    includes/OpenGLAPI/src/OpenGLAPI.hpp)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "includes" "includes/glad/include")

if(NOT TARGET OpenGLAPI)
	add_subdirectory("includes/OpenGLAPI")
endif()

target_link_libraries(${PROJECT_NAME} OpenGL::GL OpenGLAPI)
install(TARGETS ${PROJECT_NAME})
