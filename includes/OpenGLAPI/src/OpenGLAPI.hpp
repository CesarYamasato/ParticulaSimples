#ifndef OPENGLAPI_HPP
#define OPENGLAPI_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include "../includes/Shader.h"
#include "InputManager.hpp"
#include "common.hpp"
#include "SpriteRenderer.hpp"


namespace OpenGLAPI{

    ////////////////////////////////////////////////////////////////////////
    enum class RENDER_TYPE {RECTANGLE, CIRCLE, TRIANGLE};

    ////////////////////////////////////////////////////////////////////////
    enum class COLOR {RED,BLUE,GREEN,YELLOW,PURPLE};

    Shader* spriteShader;

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
}
#endif