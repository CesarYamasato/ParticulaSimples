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

        void move(float x, float y){
            transform->move(x,y);
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
    class ParticleObject : protected MoveableObject{
        public:
        ParticleObject(float x, float y, float forceX, float forceY, float timeToLive, float time, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        {
            SpawnTime = time;
            TimeToLive = timeToLive;

            this->shader = shader;
            Texture = texture;

            this->Renderer = new OpenGLAPI::SpriteRenderer(shader);

            Width = width;
            Height = height;
        }

        ~ParticleObject(){
            delete Renderer;
        }

        virtual void draw(int ResolutionX, int ResolutionY, float time, float currentTime) = 0;

        virtual void move(float time) = 0;

        protected:
        int Width,Height;
        float SpawnTime, TimeToLive;
        Shader * shader;
        OpenGLAPI::Texture * Texture;
        OpenGLAPI::SpriteRenderer * Renderer;
    };

    class FireParticle : public ParticleObject{
        FireParticle(float x, float y, float forceX, float forceY, float fadeIn,float fadeOut,float timeToLive, float time, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        : ParticleObject(x,y,forceX,forceY, timeToLive, time, shader, texture, height, width)
        {
            SpawnTime = time;
            TimeToLive = timeToLive;

            this->shader = shader;
            Texture = texture;

            this->Renderer = new OpenGLAPI::SpriteRenderer(shader);

            FadeIn = fadeIn;
            FadeOut = fadeOut;

            Width = width;
            Height = height;
        }

        void draw(int ResolutionX, int ResolutionY, float time, float currentTime){
            float x = getX();
            float y = getY();
            float opacity = 1.0;
            if(currentTime <= FadeIn+SpawnTime) opacity = 1.0-((FadeIn + SpawnTime -currentTime)/FadeIn);
            else if(currentTime >= SpawnTime+TimeToLive-FadeOut) opacity = 1.0-((SpawnTime + TimeToLive - FadeOut + currentTime)/FadeOut);

            Renderer->draw(Texture,ResolutionX, ResolutionY,x,y,Width,Height, opacity);
        }

        private:

        float FadeIn, FadeOut;

    };

    
    class Particle{
        public:
        Particle(float x, float y, int quantity, float forceX, float forceY, int angle){
            srand(time(0));
            for(int i = 0;i < quantity; i++){
                ParticleObject * ParticleInsert = new ParticleObject();
                float forceR;
                getForce(forceR, forceX, forceY, angle);
                ParticleInsert->spawn(x,y,forceX,forceY);
                list.insert(list.begin(),ParticleInsert);
            }
        }

        void Update(float ResolutionX,float ResolutionY, float time, float currentTime){
            std::list<ParticleAPI::ParticleObject*>::iterator it;
            for(it = list.begin(); it != list.end(); ++it){
                (*it)->draw(ResolutionX, ResolutionY, time, currentTime);
            }
        }

        private:

        void getForce(float &forceR, float &forceX, float& forceY, int angle){
            float angleInsert = static_cast<float>((rand()%angle));
            angleInsert = glm::radians(angleInsert);
            forceR = glm::sqrt(forceX*forceX + forceY*forceY);
            forceX = glm::sin(angleInsert)*forceR;
            forceY = glm::cos(angleInsert)*forceR;
        }

        int Width,Height;
        float Force, FadeIn, FadeOut, TimeToLive, Angle;
        std::list<ParticleObject*> list;
    };
}