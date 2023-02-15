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

class WaterParticle: public ParticleAPI::ParticleObject{
    public:
    WaterParticle(float gravity, float forceX, float timeToLive, Shader * shader, OpenGLAPI::Texture* texture, int height, int width) 
    : ParticleObject(timeToLive, shader, texture, height,width){
        Force = forceX;
        Gravity = gravity;
        first = true;
    }

    void Move (float deltaTime) override{
        if(!first) this->addVel(deltaTime);
        else {
            this->addVel(deltaTime);
            this->addForce(-Force, 0);
            first = false;
        }
        this->move(deltaTime);
    }

    ParticleObject * Spawn(float x, float y)override{
        ParticleObject* returnParticle = new WaterParticle(Gravity, Force, TimeToLive,shader, Texture, Height, Width);
        float random = getRandomForce();
        std::cout << random << std::endl;
        returnParticle->ParticleAPI::MoveableObject::addForce(Force*random, Gravity);
        returnParticle->ParticleAPI::MoveableObject::move(x,y);
        return returnParticle;
    }

    private:
    float Force, Gravity;
    bool first;

    float getRandomForce(){
        //int time  = static_cast<int>(OpenGLAPI::InputManager::getInputManager()->getTime());
        float random = static_cast<float>(rand()%2);
        for(int i = 1; i < 3;  i++) random += ((rand()%5)+5)/pow(10,i);
        return random;
    }
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

    std::string Waterfragment = path + "WaterFragment.fs";
    Shader * waterShader = new Shader(vertexPath.c_str(), Waterfragment.c_str());     

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
    WaterParticle waterParticle(-100.,50., 10.,waterShader, &Textura2, 10.,10.);
    ParticleAPI::ParticleSpawner * waterSpawner = new ParticleAPI::ParticleSpawner(300., 500., 0.1,30., &waterParticle);
    ParticleAPI::ParticleSpawner* particleSpawner = new ParticleAPI::ParticleSpawner(300., 10.,0.3,30., &Particula);
    ParticleAPI::ParticleSpawner* particleSpawner2 = new ParticleAPI::ParticleSpawner(200., 10.,0.3,30., &Particula);
    //OpenGLAPI::SpriteRenderer * campfire = new OpenGLAPI::SpriteRenderer() 
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

        if(mouse[2]) new ParticleAPI::ParticleSpawner((1.-((mouse[0]+1)/2))*Resolution[0], ((1.-(mouse[1]+1)/2))*Resolution[1],0.3,30., &Particula);
        
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