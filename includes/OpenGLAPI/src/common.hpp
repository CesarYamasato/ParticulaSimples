#ifndef OPENGLAPI_COMMON
#define OPENGLAPI_COMMON

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace OpenGLAPI{
    extern GLFWwindow * window;

    //Classe para texturas
    class Texture
    {
        public:
        Texture(unsigned int Height,unsigned int Width, GLenum WrapParams_S = GL_REPEAT, GLenum WrapParams_T = GL_REPEAT, GLenum MagParams = GL_LINEAR, GLenum MinParams = GL_LINEAR, unsigned char* data = NULL);
        
        operator GLuint();

        void bind();

        unsigned int getID();

        unsigned int getHeight();

        unsigned int getWidth();
        
        private:
        unsigned int textureID;
        unsigned int Height;
        unsigned int Width;
        unsigned char* Data;
    };
}

#endif