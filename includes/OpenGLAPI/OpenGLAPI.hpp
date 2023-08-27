#ifndef OPENGLAPI_HPP
#define OPENGLAPI_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include "../Shader.h"
#include "InputManager.hpp"
#include "common.hpp"


namespace OpenGLAPI{

    int WindowShouldClose(GLFWwindow * window);

    int libInit(int SCR_WIDTH, int SCR_HEIGHT, const char* name,void framebuffer_size_callback(GLFWwindow* window, int width, int height));

    void* initGlad(const char * name);

    void render(GLFWwindow * window);

    void setwindowTitle(const char* title);

    void Terminate();

    void setDefaultParams(Shader shader);

    void setKeyCallBackFunction(void * keyCallback(GLFWwindow*, int , int, int, int));
    
    //Função responsável por encontrar o path para um determinado diretório a partir do diretório pai
    const char* GetPathTo(std::string directory);

    void MessageCallback( GLenum source, GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam );

    //Classe responsável por ativar e desativar mensagens de debug

    class DebugManager
    {
        public:

        static DebugManager * getDebugManager();

        void DisableDebug();

        void EnableDebug();

        private:

        DebugManager();
    };


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

    //Classe para reenderizar sprites

    class SpriteRenderer{
        public:
        SpriteRenderer(Shader * shader);

        void draw(OpenGLAPI::Texture* texture,int resolutionX, int resolutionY, float x, float y,float sizex,float sizey, float opacity);

        ~SpriteRenderer();

        static int NumRenderer, DelRenderer;

        private:
        Shader * shader;
        unsigned int quadVAO;
        unsigned int VBO;

        void initRenderData();
    };
}
#endif