#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"
#include "Particle.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float vertices[] = {
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f // top left 
};

unsigned int indices[] = {
    0,1,3,
    1,2,3
};

int main()
{
    
    if (__cplusplus == 201703L)
        std::cout << "C++17" << std::endl;
    else if (__cplusplus == 201402L)
        std::cout << "C++14" << std::endl;
    else if (__cplusplus == 201103L)
        std::cout << "C++11" << std::endl;
    else if (__cplusplus == 199711L)
        std::cout << "C++98" << std::endl;
    else
        std::cout << "pre-standard C++" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Textura", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader * shader;

    std::string path = "/home/cesar/Documentos/OpenGL/Particula/Shaders/";
    std::string vertexPath = path + "ParticuleVertex2.vs";
    std::string fragmentPath = path + "ParticleFragment.fs";
    shader = new Shader(vertexPath.c_str(), fragmentPath.c_str());        

    std::string imagePath = "/home/cesar/Documentos/OpenGL/Particula/Skarmory.gif"; 
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);   

    std::cout << "CHANNELS: "<<nrChannels<< std::endl;                           

    //Habilita mensagens de debug

    OpenGLAPI::DebugManager* debugManager = nullptr;
    debugManager = OpenGLAPI::DebugManager::getDebugManager();
    //debugManager->DisableDebug();

    OpenGLAPI::InputManager * inputManager = nullptr;
    inputManager = OpenGLAPI::InputManager::getInputManager();
  
    int frame = 0;

    inputManager->processInput(window);

    OpenGLAPI::Texture Textura(height, width, GL_REPEAT,GL_REPEAT, GL_LINEAR, GL_LINEAR, data);

    OpenGLAPI::Texture Textura2(height, width);
    
    ParticleAPI::ParticleObject Particula(10., 2., 50.,inputManager->getTime(), shader, &Textura,20., 20.);
    ParticleAPI::Particle ParticleSpawner(300., 400.,&Particula, 5, 500.,0., 70);
    std::cout << "BACK AT MAIN" << std::endl;
    //Particula.spawn(300., 400.,200.,0.);

    int * Resolution = (int*) malloc(sizeof(int)*2);

    float current = inputManager->getTime();
    float after = current;
    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!glfwWindowShouldClose(window)){
        //Coletando a informação da textura atualmente ativa e copiando para um espaço de memória
        inputManager->processInput(window);
        Resolution = inputManager->getResolution();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //std::cout << "TIME: " <<inputManager->getTime()<< std::endl;
        
        //Particula.draw(800,600, after-current, current);
        ParticleSpawner.Update(800,600, after-current, current);

        current = after;
        after = inputManager->getTime();

        glfwSwapBuffers(window);
        glfwPollEvents();
        frame++;
    }

    glfwTerminate();
    return 0;
}

//callback: chamada quando um determinado evento acontece
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    printf("%i:%i\n", width,height);
}