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

    //Particle object class
    class ParticleObject : protected MoveableObject{
        public:
        ParticleObject(float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        {
            float time = OpenGLAPI::InputManager::getInputManager()->getTime();
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

        virtual void Draw(int ResolutionX, int ResolutionY) {
            float x = getX();
            float y = getY();
             Renderer->draw(Texture,ResolutionX, ResolutionY,x,y,Width,Height, 1.0);
        }

        virtual void Move(float deltaTime) = 0;

        virtual ParticleObject* Spawn(float x, float y) = 0;

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
        public:
        FireParticle(float fadeIn,float fadeOut,float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
        : ParticleObject(timeToLive, shader, texture, height, width)
        {
            FadeIn = fadeIn;
            FadeOut = fadeOut;
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

        void Move(float deltaTime) override{
            int time = static_cast<int>(OpenGLAPI::InputManager::getInputManager()->getTime());
            srand(time);
            int random = rand()%2;
            int signal = (rand()%2) - 1;
            if(signal == 0) signal = 1;
            MoveableObject::move(random*Width*signal, Height);
        }

        ParticleObject* Spawn(float x, float y) override{
            ParticleObject* particle = new FireParticle(FadeIn, FadeOut, TimeToLive, shader, Texture, Height, Width);
            particle->MoveableObject::move(x,y);
            return particle;
        }

        private:

        float FadeIn, FadeOut;

    };

     //Class that receives a particle and spawns particles of that same type
    class ParticleSpawner{
        public:
        ParticleSpawner(float x, float y, int quantity, float timeToLive, ParticleObject* particle){
            ID = ParticleManager::getParticleManager()->Insert(this);
            float time = OpenGLAPI::InputManager::getInputManager()->getTime();
            TimeToLive = timeToLive;
            SpawnTime = time;
            Particle = particle;
            this->x = x;
            this->y = y;
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

        void checkTimeToLive(){
            float time = OpenGLAPI::InputManager::getInputManager()->getTime();
            if((SpawnTime + TimeToLive) < time) delete(this);
        }

        virtual void Spawn(){
            float time = OpenGLAPI::InputManager::getInputManager()->getTime();
            if(time > LastSpawn + 2) list.push_front(Particle->Spawn(x,y));
        }

        ~ParticleSpawner(){
            ParticleManager::getParticleManager()->deleteSpawner(this);
        }

        bool operator == (ParticleSpawner * other){
            return this->ID == other->ID;
        }

        private:
        int ID;
        int Width,Height;
        float Angle, TimeToLive, SpawnTime, LastSpawn, x, y;
        ParticleObject* Particle;
        std::list<ParticleObject*> list;
        std::list<ParticleObject*>::iterator it;
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
            return quantity;
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
                (*it)->checkTimeToLive();
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
}