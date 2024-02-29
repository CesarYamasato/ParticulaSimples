#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "includes/stb_image/stb_image.h"
#include "includes/SimpleParticle/Particle.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>
#include <execinfo.h>
#include <signal.h>

using namespace OpenGLAPI;
using namespace Manager;
using namespace Renderer;

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

class FireworkParticle: public ParticleAPI::ParticleObject{

};

class FireworkParticleSpawner: public ParticleAPI::ParticleSpawner{
    public:
    FireworkParticleSpawner(float x, float y, FireworkParticle* particle): ParticleSpawner(x,y,0,100,particle){
        renderer = new SpriteRenderer();
    }
    private:
    SpriteRenderer * renderer;
    //ParticleAPI::ParticleSpawner fireSpawner;
};

// class WaterParticle: public ParticleAPI::ParticleObject{
//     public:
//     WaterParticle(float gravity, float forceX, float timeToLive, Shader * shader, OpenGLAPI::Texture* texture, int height, int width) 
//     : ParticleObject(timeToLive, shader, texture, height,width){
//         Force = forceX;
//         Gravity = gravity;
//         first = true;
//     }

//     void Move (float deltaTime) override{
//         if(!first) this->addVel(deltaTime);
//         else {
//             this->addVel(deltaTime);
//             this->addForce(-Force, 0);
//             first = false;
//         }
//         this->move(deltaTime);
//     }

//     ParticleObject * Spawn(float x, float y)override{
//         ParticleObject::Pcount++;
//         ParticleObject* returnParticle = new WaterParticle(Gravity, Force, TimeToLive,shader, Texture, Height, Width);
//         float random = getRandomForce();
//         returnParticle->ParticleAPI::MoveableObject::addForce(Force*random, Gravity);
//         returnParticle->ParticleAPI::MoveableObject::move(x,y);
//         return returnParticle;
//     }

//     private:
//     float Force, Gravity;
//     bool first;

//     float getRandomForce(){
//         //int time  = static_cast<int>(OpenGLAPI::InputManager::getInputManager()->getTime());
//         float random = static_cast<float>(rand()%2);
//         for(int i = 1; i < 3;  i++) random += ((rand()%5)+5)/pow(10,i);
//         return random;
//     }
// };

int main(int argc, char **argv)
{

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if(!OpenGLAPI::libInit(SCR_WIDTH,SCR_HEIGHT, "teste", framebuffer_size_callback)) return -1;
    
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    if(argc > 1){
        if(!strcmp(argv[1], "--debug")){
            DebugManager::getDebugManager()->EnableDebug();
        }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader * shader;
    Shader * shaderSprite;
    std::string *shaderPath = new std::string(OpenGLAPI::GetPathTo("/Shaders/"));
    std::string vertexPath = *shaderPath + "ParticuleVertex2.vs";
    std::string fragmentPath = *shaderPath + "ParticleFragment.fs";
    std::string fragmentPathSprite = *shaderPath + "Sprite.fs";
    shader = new Shader(vertexPath.c_str(), fragmentPath.c_str()); 
    shaderSprite = new Shader(vertexPath.c_str(), fragmentPathSprite.c_str()); 

    std::string Waterfragment = *shaderPath + "WaterFragment.fs";
    Shader * waterShader = new Shader(vertexPath.c_str(), Waterfragment.c_str());     

    delete(shaderPath);

    std::string imagePath = OpenGLAPI::GetPathTo("/campfire.png"); 
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);                       

    InputManager * inputManager = InputManager::getInputManager();

    OpenGLAPI::Texture Textura(height, width, GL_REPEAT,GL_REPEAT, GL_LINEAR, GL_LINEAR, data);

    ParticleAPI::ParticleManager* particleManager = ParticleAPI::ParticleManager::getParticleManager();

    ParticleAPI::FireParticle Particula(10,10,5.0,5.0,10.0, 300., 20., shader, &Textura, 20.0,20.0);

    ParticleAPI::ParticleSpawner* particleSpawner2 = new ParticleAPI::ParticleSpawner(365, 270,0.2,500., &Particula);


    SpriteRenderer * renderer = new SpriteRenderer(shaderSprite);

    int Resolution[2];

    int frame = 0;
    int tempFrame = 0;
    float fps;
    char fpstitle[50];

    float current = glfwGetTime();
    float after = current;
    float old, newT;
    float diff = 0;
    old = glfwGetTime();

    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!OpenGLAPI::WindowShouldClose(OpenGLAPI::window)){
        //Coletando a informação da textura atualmente ativa e copiando para um espaço de memória
        newT = glfwGetTime();
        inputManager->Update();
        double* mouse = inputManager->getMouse();
        glfwGetFramebufferSize(OpenGLAPI::window,&Resolution[0], &Resolution[1]);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        if(mouse[2]) new ParticleAPI::ParticleSpawner(((mouse[0]+1)/2)*Resolution[0], ((mouse[1]+1)/2)*Resolution[1],0.3,10., &Particula);

        renderer->draw(400, 300, 100.0f, 100.0f, 1.0f, &Textura);
        
        particleManager->Update(after-current);

        current = after;
        after = glfwGetTime();
        diff = newT-old;

        if(diff > .2){
            old = newT;
            fps = tempFrame/diff;
            sprintf(fpstitle, "FPS = %.2f", fps);
            OpenGLAPI::setwindowTitle(fpstitle);
            tempFrame = 0;
        }
        OpenGLAPI::render(OpenGLAPI::window);
        frame++;
        tempFrame++;
    }
    // std::cout
    // << " PCOUNT " << OpenGLAPI::SpriteRenderer::NumRenderer
    // << " PDCOUNT " <<ParticleAPI::ParticleObject::Pcount
    // << " CCOUNT " << ParticleAPI::Transform::numDeletes
    // << " CCOUNT " << ParticleAPI::Transform::numCreates;
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