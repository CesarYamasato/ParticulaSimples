#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"
#include "includes/ParticleAPI/Particle.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>
#include <execinfo.h>
#include <signal.h>

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
        ParticleObject::Pcount++;
        ParticleObject* returnParticle = new WaterParticle(Gravity, Force, TimeToLive,shader, Texture, Height, Width);
        float random = getRandomForce();
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

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}



int main(int argc, char **argv)
{
   signal(SIGSEGV, handler);

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


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if(!OpenGLAPI::libInit(SCR_WIDTH,SCR_HEIGHT, "teste", framebuffer_size_callback)) return -1;
    std::cout << "HELLO";
    
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    Shader * shader;
    std::string *shaderPath = new std::string(OpenGLAPI::GetPathTo("/Shaders/"));
    std::string vertexPath = *shaderPath + "ParticuleVertex2.vs";
    std::string fragmentPath = *shaderPath + "ParticleFragment.fs";
    shader = new Shader(vertexPath.c_str(), fragmentPath.c_str()); 

    std::string Waterfragment = *shaderPath + "WaterFragment.fs";
    Shader * waterShader = new Shader(vertexPath.c_str(), Waterfragment.c_str());     

    delete(shaderPath);

    std::string imagePath = OpenGLAPI::GetPathTo("/skarmory.gif"); 
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);                       

    //Habilita mensagens de debug

    //OpenGLAPI::DebugManager* debugManager = nullptr;
    //debugManager = OpenGLAPI::DebugManager::getDebugManager();
    //debugManager->DisableDebug();

    //glEnable(GL_DEBUG_OUTPUT );
    //glDebugMessageCallback(OpenGLAPI::MessageCallback, 0 );


    OpenGLAPI::InputManager * inputManager = nullptr;
    inputManager = OpenGLAPI::InputManager::getInputManager();
    inputManager->setWindow(OpenGLAPI::window);
    inputManager->setKeyCallBackFunction();

    inputManager->processInput();

    OpenGLAPI::Texture Textura(height, width, GL_REPEAT,GL_REPEAT, GL_LINEAR, GL_LINEAR, data);

    //OpenGLAPI::Texture Textura2(height, width);

    ParticleAPI::ParticleManager* particleManager = ParticleAPI::ParticleManager::getParticleManager();

    ParticleAPI::FireParticle Particula(5.0,5.0,10.0,shader, &Textura, 10.0,10.0);
    //WaterParticle waterParticle(-100.,50., 10.,waterShader, &Textura2, 10.,10.);
    //ParticleAPI::ParticleSpawner * waterSpawner = new ParticleAPI::ParticleSpawner(300., 500., 0.1,30., &waterParticle);
    //ParticleAPI::ParticleSpawner* particleSpawner = new ParticleAPI::ParticleSpawner(300., 5.,0.3,30., &Particula);
    ParticleAPI::ParticleSpawner* particleSpawner2 = new ParticleAPI::ParticleSpawner(200., 5.,0.3,10., &Particula);
    //OpenGLAPI::SpriteRenderer * campfire = new OpenGLAPI::SpriteRenderer() 
    //Particula.spawn(300., 400.,200.,0.);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    double * Resolution = (double*) malloc(sizeof(int)*2);

    int frame = 0;
    int tempFrame = 0;
    float fps;
    char fpstitle[50];

    float current = inputManager->getTime();
    float after = current;
    float old, newT;
    float diff = 0;
    old = inputManager->getTime();

    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!OpenGLAPI::WindowShouldClose(OpenGLAPI::window)){
        //Coletando a informação da textura atualmente ativa e copiando para um espaço de memória
        newT = inputManager->getTime();
        inputManager->processInput();
        double* mouse = inputManager->getMouse();
        Resolution = inputManager->getResolution();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //std::cout << "TIME: " <<inputManager->getTime()<< std::endl;

        //std::cout << "MOUSE: " << ((mouse[0]+1)/2) << " " << ((mouse[1]+1)/2) << std::endl;

        if(mouse[2]) new ParticleAPI::ParticleSpawner((1.-((mouse[0]+1)/2))*Resolution[0], ((1.-(mouse[1]+1)/2))*Resolution[1],0.3,10., &Particula);
        
        particleManager->Update(after-current);
        particleManager->Draw(Resolution[0],Resolution[1]);

        current = after;
        after = inputManager->getTime();
        diff = newT-old;

        if(diff > .2){
            old = newT;
            fps = tempFrame/diff;
            sprintf(fpstitle, "FPS = %.2f", fps);
            OpenGLAPI::setwindowTitle(OpenGLAPI::window,fpstitle);
            tempFrame = 0;
        }
        OpenGLAPI::render(OpenGLAPI::window);
        frame++;
        tempFrame++;
    }
    std::cout
    << " PCOUNT " << OpenGLAPI::SpriteRenderer::NumRenderer
    << " PDCOUNT " <<OpenGLAPI::SpriteRenderer::DelRenderer
    << " CCOUNT " << ParticleAPI::ParticleSpawner::Dcount
    << " CCOUNT " << ParticleAPI::ParticleManager::SpawnerCount;
    free(data);
    OpenGLAPI::Terminate();
    return 0;
}

//callback: chamada quando um determinado evento acontece
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    printf("%i:%i\n", width,height);
}