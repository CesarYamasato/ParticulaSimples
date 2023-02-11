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

    class ParticleManager{
        public:

        static ParticleManager * getParticleManager(){
            static ParticleManager* manager = nullptr;
            if(!manager) manager = new ParticleManager();
            return manager;
        }

        //Function that is called everytime a ParticleSpawner is created 
        int Insert(ParticleSpawner* spawner){
            list.insert(list.begin(), spawner);
            quantity++;
        }

        //Function that is called everytime a ParticleSpawner is deleted
        void deleteSpawner(ParticleSpawner* spawner){
            list.remove(spawner);
            quantity--;
        }

        //Function that is called every frame in order to update position
        void Update(float deltaTime){
            std::list<ParticleSpawner*>::iterator it;
            for(it = list.begin(); it != list.end(); ++it){
                (*it)->Update(deltaTime);
            }
        }

        //Function that is called every frame in order to draw the particles
        void Draw(int ResolutionX,int ResolutionY){
            std::list<ParticleSpawner*>::iterator it;
            for(it = list.begin(); it != list.end(); ++it){
                (*it)->Draw(ResolutionX,ResolutionY);
            }
        }

        private:
        int quantity;
        std::list<ParticleSpawner*> list;

        ParticleManager(){
            quantity = 0;
        }
    };

    //Class that receives a particle and spawns particles of that same type
    class ParticleSpawner{
        public:
        ParticleSpawner(float x, float y, int quantity, float timeToLive){
            ID = ParticleManager::getParticleManager()->Insert(this);
        }

        virtual void Update(float deltaTime){
            for(it = list.begin(); it != list.end(); ++it){
                if((*it)->shoudlDie()){
                    list.erase(it);
                }
                (*it)->Move(deltaTime);
            }
        }

        virtual void Draw(int ResolutionX,int ResolutionY){
            for(it = list.begin(); it != list.end(); ++it){
                (*it)->Draw(ResolutionX, ResolutionY);
            }
        }

        virtual void Spawn() = 0;

        ~ParticleSpawner(){
            ParticleManager::getParticleManager()->deleteSpawner(this);
        }

        bool operator == (ParticleSpawner * other){
            return this->ID == other->ID;
        }

        private:
        int ID;
        int Width,Height;
        float Angle, TimeToLive;
        std::list<ParticleObject*> list;
        std::list<ParticleObject*>::iterator it;
        
    };

    //Particle object class
    class ParticleObject : protected MoveableObject{
        public:
        ParticleObject(float timeToLive, float time, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
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

        bool shoudlDie(){
            float time = OpenGLAPI::InputManager::getInputManager()->getTime();
            return (SpawnTime + TimeToLive) < time;
        }

        virtual void Draw(int ResolutionX, int ResolutionY) = 0;

        virtual void Move(float time) = 0;

        bool operator == (ParticleObject* other){
            return this->ID == other->ID;
        }

        protected:
        int ID;
        int Width,Height;
        float SpawnTime, TimeToLive;
        Shader * shader;
        OpenGLAPI::Texture * Texture;
        OpenGLAPI::SpriteRenderer * Renderer;
    };

    class FireParticle : public ParticleObject{
        FireParticle(float x, float y, float forceX, float forceY, float fadeIn,float fadeOut,float timeToLive, float time, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        : ParticleObject(timeToLive, time, shader, texture, height, width)
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

        void Draw (int ResolutionX, int ResolutionY) override{
            float x = getX();
            float y = getY();
            float opacity = 1.0;
            float currentTime = OpenGLAPI::InputManager::getInputManager()->getTime();
            if(currentTime <= FadeIn+SpawnTime) opacity = 1.0-((FadeIn + SpawnTime -currentTime)/FadeIn);
            else if(currentTime >= SpawnTime+TimeToLive-FadeOut) opacity = 1.0-((SpawnTime + TimeToLive - FadeOut + currentTime)/FadeOut);

            Renderer->draw(Texture,ResolutionX, ResolutionY,x,y,Width,Height, opacity);
        }

        private:

        float FadeIn, FadeOut;

    };
}