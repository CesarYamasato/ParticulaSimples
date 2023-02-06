#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"
#include "OpenGLAPI.hpp"

////////////////////////////////////////////////////////////////////////
enum class RENDER_TYPE {RECTANGLE, CIRCLE, TRIANGLE};

////////////////////////////////////////////////////////////////////////
enum class COLOR {RED,BLUE,GREEN,YELLOW,PURPLE};

namespace ParticleAPI{

    ////////////////////////////////////////////////////////////////////////
    unsigned int SquareIndices[] = {
        0,1,3,
        1,2,3
    };

    ////////////////////////////////////////////////////////////////////////
    class Transform{
        public:
        Transform(float x = 0, float y = 0){
            this->x = x;
            this->y = y;
        }

        float getX(){
            return x;
        }

        float getY(){
            return y;
        }

        void move(float x, float y){
            this->x += x;
            this->y += y;
        }

        private:
        float x,y;
    };

    ////////////////////////////////////////////////////////////////////////
    class Collider{

    };

    ////////////////////////////////////////////////////////////////////////
    class GameObject{
        public:

        GameObject(float x =0, float y =0){
            
        }
    };

    ////////////////////////////////////////////////////////////////////////
    class MoveableObject{
        public:

        MoveableObject(float X = 0., float Y = 0.,float velX = 0, float velY = 0, float accX = 0,float accY = 0, float mass = 1){
            transform = new ParticleAPI::Transform(X,Y);
            std::cout << X << "AQUI" << Y << std::endl;
            VelX = velX;
            VelY = velY;
            AccX = accX;
            AccY = accY;
            Mass = mass;
        }

        void move(float time){
            transform->move(VelX*time,VelY*time);
        }

        void addVel(float time){
            VelX += AccX*time;
            VelY += AccY*time;
        }

        void addForce(float forceX,float forceY){
            AccX += forceX/Mass;
            AccY += forceY/Mass;
        }

        float getX(){
            return transform->getX();
        }

        float getY(){
            return transform->getY();
        }

        private:
        float VelX, VelY;
        float AccX, AccY;
        float Mass;
        Transform * transform;
    };

    ////////////////////////////////////////////////////////////////////////
    class ParticleObject : private MoveableObject{
        public:
        ParticleObject(float x, float y,float forceX, float forceY, float fadeIn,float fadeOut,float timeToLive, float time, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        : MoveableObject(x, y)
        {
            addForce(forceX, forceY);
            SpawnTime = time;
            TimeToLive = timeToLive;

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            this->shader = shader;
            Texture = texture;

            this->Renderer = new OpenGLAPI::SpriteRenderer(shader);

            FadeIn = fadeIn;
            FadeOut = fadeOut;

            Width = width;
            Height = height;

            first = true;
        }

        void draw(int ResolutionX, int ResolutionY, float time, float currentTime){
            float x = getX();
            float y = getY();
            float opacity = 1.0;
            if(currentTime <= FadeIn+SpawnTime) opacity = 1.0-((FadeIn + SpawnTime -currentTime)/FadeIn);
            else if(currentTime >= SpawnTime+TimeToLive-FadeOut) opacity = 1.0-((SpawnTime + TimeToLive - FadeOut + currentTime)/FadeOut);
            /*float vertices [] = {
                x, y,
                x, y-Height,
                x-Width, y-Height,
                x-Width, y
            };

            std::cout << "opacity: " << opacity << "(X,Y): "<< x/ResolutionX << "," << -y/ResolutionY <<std::endl;
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ParticleAPI::SquareIndices),ParticleAPI::SquareIndices, GL_STREAM_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),(void *)0 );
            glEnableVertexAttribArray(0);

            shader->use();
            shader->setVec2("Resolution",ResolutionX, ResolutionY);
            shader->setVec2("TextResolution", Width,Height);
            shader->setFloat("Opacity", opacity);
            
            glActiveTexture(GL_TEXTURE0);
            Texture->bind();

            shader->setSampler("Texture",0);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

            glBindTexture(GL_TEXTURE_2D,0);

            glBindVertexArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

            Renderer->draw(Texture,ResolutionX, ResolutionY,x,y, opacity);

            
            addVel(time);
            addForce(0.f,-10.f);
            move(time);

            first = false;
        }

        private:
        int Width,Height;
        bool first;
        float FadeIn, FadeOut, SpawnTime, TimeToLive;
        Shader * shader;
        OpenGLAPI::Texture * Texture;
        OpenGLAPI::SpriteRenderer * Renderer;
        unsigned int VBO, VAO, EBO;
    };

    
    class Particle : private MoveableObject{
        public:
        Particle(int quantity, int width, int height, float forceX, float forceY , float fadeIn, float fadeOut, float timeToLive, float angle, const char * texture, float x = 0, float y = 0, RENDER_TYPE = RENDER_TYPE::RECTANGLE){
            std::string path = "/home/cesar/Documentos/OpenGL/Particula/Shaders/";
            std::string vertexPath = path + "ParticleVertex";
            std::string fragmentPath = path + "ParticleFragment";
            shader = new Shader(vertexPath.c_str(), fragmentPath.c_str());
        }

        Particle(int quantity, int width, int height,  float forceX, float forceY, float fadeIn, float fadeOut, float timeToLive, float angle, COLOR = COLOR::RED, float x = 0, float y = 0, RENDER_TYPE = RENDER_TYPE::RECTANGLE){

        }

        private:
        int Width,Height;
        float Force, FadeIn, FadeOut, TimeToLive, Angle;
        Shader * shader;
    };
}