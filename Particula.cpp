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
void MessageCallback( GLenum source, GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam );


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

    std::string imagePath = "/home/cesar/Documentos/OpenGL/Particula/orange.png"; 
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);   

    std::cout << "CHANNELS: "<<nrChannels<< std::endl;                           

    //Habilita mensagens de debug

    //OpenGLAPI::DebugManager* debugManager = nullptr;
    //debugManager = OpenGLAPI::DebugManager::getDebugManager();
    //debugManager->DisableDebug();

    glEnable(GL_DEBUG_OUTPUT );
    glDebugMessageCallback(MessageCallback, 0 );

    OpenGLAPI::InputManager * inputManager = nullptr;
    inputManager = OpenGLAPI::InputManager::getInputManager();
  
    int frame = 0;

    inputManager->processInput(window);

    OpenGLAPI::Texture Textura(height, width, GL_REPEAT,GL_REPEAT, GL_LINEAR, GL_LINEAR, data);

    OpenGLAPI::Texture Textura2(height, width);

    ParticleAPI::ParticleManager* particleManager = ParticleAPI::ParticleManager::getParticleManager();

    ParticleAPI::FireParticle Particula(5.0,5.0,10.0,shader, &Textura, 10.0,10.0);
    ParticleAPI::ParticleSpawner* particleSpawner = new ParticleAPI::ParticleSpawner(300., 10.,10,30., &Particula);
    ParticleAPI::ParticleSpawner* particleSpawner2 = new ParticleAPI::ParticleSpawner(200., 10.,10,30., &Particula);
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
        double* mouse = inputManager->getMouse();
        Resolution = inputManager->getResolution();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //std::cout << "TIME: " <<inputManager->getTime()<< std::endl;

        //std::cout << "MOUSE: " << ((mouse[0]+1)/2) << " " << ((mouse[1]+1)/2) << std::endl;

        if(mouse[2]) new ParticleAPI::ParticleSpawner((1.-((mouse[0]+1)/2))*Resolution[0], ((1.-(mouse[1]+1)/2))*Resolution[1],10,5., &Particula);
        
        particleManager->Update(after-current);
        particleManager->Draw(Resolution[0],Resolution[1]);

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

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
        {
        fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
        switch (source)
            {
                case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
                case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
            } std::cout << std::endl;
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
                case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
                case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
            } std::cout << std::endl;

            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
                case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
            } std::cout << std::endl;
            std::cout << std::endl;
        }