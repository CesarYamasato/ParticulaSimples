#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image/stb_image.h"
#include "OpenGLAPI.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>

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

    std::filesystem::path path = std::filesystem::current_path();
    const std::string pathString = path.string();

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

    //Habilita mensagens de debug

    OpenGLAPI::DebugManager* debugManager = nullptr;
    debugManager = OpenGLAPI::DebugManager::getDebugManager();
    //debugManager->DisableDebug();

    std::string parentPath = (path.parent_path()).c_str();

    const std::string vertexShader = "/Shaders/VertexShader.vs";
    const std::string vertexPath = parentPath + vertexShader;
    const char * vertexShaderPath = vertexPath.c_str();

    const std::string A = "/Shaders/BufferA.fs";
    const std::string APath = parentPath + A;
    const char * AShaderPath = APath.c_str();

    const std::string B = "/Shaders/BufferB.fs";
    const std::string BPath = parentPath + B;
    const char * BShaderPath = BPath.c_str();

    const std::string C = "/Shaders/BufferC.fs";
    const std::string CPath = parentPath + C;
    const char * CShaderPath = CPath.c_str();

    const std::string Image = "/Shaders/image.fs";
    const std::string IPath = parentPath + Image;
    const char * IShaderPath = IPath.c_str();

    Shader ShaderprogramA(vertexShaderPath,AShaderPath);
    Shader ShaderprogramB(vertexShaderPath,BShaderPath);
    Shader ShaderprogramC(vertexShaderPath,CShaderPath);
    Shader ShaderProgramImage(vertexShaderPath,IShaderPath);


    ////////////////////
    // OPENGL BÁSICOS //
    ////////////////////
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void *)0 );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void *) (3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    ///////////////////////////
    //        BufferA        //
    ///////////////////////////

    OpenGLAPI::Texture bufferA(SCR_HEIGHT,SCR_WIDTH);

    ///////////////////////////
    //        BufferB        //
    ///////////////////////////

    OpenGLAPI::Texture bufferB(SCR_HEIGHT, SCR_WIDTH);

    ///////////////////////////
    //        Bufferc        //
    ///////////////////////////

    OpenGLAPI::Texture bufferC(SCR_HEIGHT, SCR_WIDTH);


    ////////////////////
    //  FRAMEBUFFER   //
    ////////////////////
    //glBindTexture(GL_TEXTURE_2D, bufferA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferA);
    //glBindTexture(GL_TEXTURE_2D, bufferB);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bufferB);
    //glBindTexture(GL_TEXTURE_2D, bufferC);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, bufferC);
    //glBindTexture(GL_TEXTURE_2D,0);
    
    std::cout << " bufferA: " << (GLuint) bufferA << " bufferB: " << (GLuint) bufferB << " bufferC: " << (GLuint) bufferC << std::endl;

    //Criando o framebuffer
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);

    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);     


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferA, 0 );


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer A is not complete!" << std::endl;

    ////////////////////
    // FRAMEBUFFER B  //
    ////////////////////

     

    //Criando o framebuffer
    unsigned int FBOB;
    glGenFramebuffers(1, &FBOB);

    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBOB);     


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferB, 0 );


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer B is not complete!" << std::endl;

    ////////////////////
    // FRAMEBUFFER C  //
    ////////////////////

     

    //Criando o framebuffer
    unsigned int FBOC;
    glGenFramebuffers(1, &FBOC);

    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBOC);     


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferC, 0 );


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer C is not complete!" << std::endl;

    OpenGLAPI::InputManager * inputManager = nullptr;
    inputManager = OpenGLAPI::InputManager::getInputManager();
  
    int frame = 0;

    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!glfwWindowShouldClose(window)){
        //Coletando a informação da textura atualmente ativa e copiando para um espaço de memória
        inputManager->processInput(window);
        void * arrayInput = inputManager->getInput();
        
        //Renderizando para o framebuffer criado
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bufferA);
        //glBindTexture(GL_TEXTURE_2D, bufferB);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bufferB);
        //glBindTexture(GL_TEXTURE_2D, bufferC);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bufferC);

        //Utilização de um segundo shader para calcular vizinhança
        ShaderprogramA.use();
        ShaderprogramA.setInt("Frame", frame);
        ShaderprogramA.setSampler("bufferB",1);
        ShaderprogramA.setDefaultParams(arrayInput);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, FBOB);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bufferA);
        //glBindTexture(GL_TEXTURE_2D, bufferB);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bufferB);
        //glBindTexture(GL_TEXTURE_2D, bufferC);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bufferC);

        ShaderprogramB.use();
        ShaderprogramB.setSampler("bufferA",0);
        ShaderprogramB.setDefaultParams(arrayInput);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, FBOC);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bufferA);
        //glBindTexture(GL_TEXTURE_2D, bufferB);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bufferB);
        //glBindTexture(GL_TEXTURE_2D, bufferC);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bufferC);

        ShaderprogramC.use();
        ShaderprogramC.setSampler("bufferA",0);
        ShaderprogramC.setDefaultParams(arrayInput);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //////////////////////////////////////////

        //Renderizando para a tela
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bufferA);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bufferB);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bufferC);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ShaderProgramImage.use();
        ShaderProgramImage.setSampler("bufferA",0);
        ShaderProgramImage.setSampler("bufferB",1);
        ShaderProgramImage.setSampler("bufferC",2);

        ShaderProgramImage.setDefaultParams(arrayInput);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        frame++;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteFramebuffers(1, &FBO);

    glfwTerminate();
    return 0;
}

//callback: chamada quando um determinado evento acontece
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    printf("%i:%i\n", width,height);
}