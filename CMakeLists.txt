cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_link_options(-mwindows)
#add_compile_options(-g -rdynamic)
#SET(GCC_COVERAGE_COMPILE_FLAGS "-g -rdynamic")

project(Particle) 

SET(SOURCES src/Particula.cpp includes/glad/src/glad.c includes/glad/include/glad/glad.h includes/glad/include/KHR/khrplatform.h
            includes/glfw/glfw3.h
	    includes/glm/glm.hpp
	    includes/stb_image/stb_image.cpp 
	    includes/Shader.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC includes)

find_package(OpenGL REQUIRED)

if(NOT TARGET SimpleParticle)
	add_subdirectory("includes/SimpleParticle")
endif()

if(NOT TARGET OpenGLAPI)
	add_subdirectory("includes/OpenGLAPI")
endif()

#target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}" "${PROJECT_BINARY_DIR}/includes/OpenGLAPI" "${PROJECT_BINARY_DIR}/includes/SimpleParticle")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${CMAKE_DL_LIBS} OpenGLAPI SimpleParticle)
