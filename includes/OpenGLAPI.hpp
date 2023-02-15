#ifndef OPENGLAPI_HPP
#define OPENGLAPI_HPP
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"


namespace OpenGLAPI{

    void MessageCallback( GLenum source, GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam );

    //Classe responsável por manejar input/informações externas ao programa
    class InputManager{
        public:
        static InputManager * getInputManager();

        void processInput(GLFWwindow *window);

        void * getInput();

        int * getResolution();

        float getTime();

        double * getMouse();

        private:

        double* mouse;
        bool shouldDraw;
        bool press;
        float time;
        int height;
        int width;


        void* array;

        InputManager();

        void getMouseValue(GLFWwindow *window);

        void getKeysPressed(GLFWwindow *window);

        void getInputArray();
    };


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
    };

    //Classe para reenderizar sprites

    class SpriteRenderer{
        public:
        SpriteRenderer(Shader * shader);

        void draw(OpenGLAPI::Texture* texture,int resolutionX, int resolutionY, float x, float y,float sizex,float sizey, float opacity);

        private:
        Shader * shader;
        unsigned int quadVAO;
        unsigned int VBO;

        void initRenderData();
    };
}
#endif