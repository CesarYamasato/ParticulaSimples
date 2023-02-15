cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

project(OpenGL)

find_package(Boost 1.71.0 COMPONENTS regex)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(OpenGL stb_image/stb_image.cpp Shader.h Particle.hpp Particle.cpp OpenGLAPI.hpp OpenGLAPI.cpp glad.c Particula.cpp) 
    target_link_libraries(OpenGL ${Boost_LIBRARIES} GL dl glfw)
endif()
