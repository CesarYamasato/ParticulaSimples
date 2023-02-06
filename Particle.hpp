#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <cstdlib>
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
    class ParticleObject{
        public:
        ParticleObject(float fadeIn,float fadeOut,float timeToLive, float time, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        {
            std::cout << "NEW PARTICLE" << std::endl;
            SpawnTime = time;
            TimeToLive = timeToLive;

            this->shader = shader;
            Texture = texture;

            this->Renderer = new OpenGLAPI::SpriteRenderer(shader);

            FadeIn = fadeIn;
            FadeOut = fadeOut;

            Width = width;
            Height = height;

            first = true;
        }

        ParticleObject * Clone(){
            ParticleObject * returnP = new ParticleObject(FadeIn, FadeOut, TimeToLive, SpawnTime, shader, Texture, Height, Width);
            return returnP;
        }

        ~ParticleObject(){
            delete Object;
            delete Renderer;
        }

        void spawn(float x, float y, float forceX, float forceY)
        {
            Object = new ParticleAPI::MoveableObject(x,y);
            Object->addForce(forceX, forceY);
            std::cout<< "SPAWNING" << std::endl;
        }

        void draw(int ResolutionX, int ResolutionY, float time, float currentTime){
           if(!Object) return;
            float x = Object->getX();
            float y = Object->getY();
            float opacity = 1.0;
            if(currentTime <= FadeIn+SpawnTime) opacity = 1.0-((FadeIn + SpawnTime -currentTime)/FadeIn);
            else if(currentTime >= SpawnTime+TimeToLive-FadeOut) opacity = 1.0-((SpawnTime + TimeToLive - FadeOut + currentTime)/FadeOut);

            Renderer->draw(Texture,ResolutionX, ResolutionY,x,y,Width,Height, opacity);
            
            Object->addVel(time);
            Object->addForce(0.f,-10.f);
            Object->move(time);

            first = false;
        }

        private:
        int Width,Height;
        bool first;
        float FadeIn, FadeOut, SpawnTime, TimeToLive;
        Shader * shader;
        ParticleAPI::MoveableObject* Object;
        OpenGLAPI::Texture * Texture;
        OpenGLAPI::SpriteRenderer * Renderer;
    };

    
    class Particle{
        public:
        Particle(float x, float y, ParticleObject * particle, int quantity, float forceX, float forceY, int angle){
            for(int i = 0;i < quantity; i++){
                ParticleObject * ParticleInsert = particle->Clone();
                int angleInsert = rand()%(angle+10) + 10;
                float forceR = glm::sqrt(forceX*forceX + forceY*forceY);
                float forceXR = glm::sin(angleInsert)*forceR;
                float forceYR = glm::cos(angleInsert)*forceR;
                ParticleInsert->spawn(x,y,forceXR,forceYR);
                list.insert(list.begin(),ParticleInsert);
                std::cout << "HELLO" << i << std::endl;
            }
            std::cout << "OUTSIDE" << std::endl;
        }

        void Update(float ResolutionX,float ResolutionY, float time, float currentTime){
            std::list<ParticleAPI::ParticleObject*>::iterator it;
            for(it = list.begin(); it != list.end(); ++it){
                (*it)->draw(ResolutionX, ResolutionY, time, currentTime);
            }
        }

        private:

        int Width,Height;
        float Force, FadeIn, FadeOut, TimeToLive, Angle;
        std::list<ParticleObject*> list;
    };
}