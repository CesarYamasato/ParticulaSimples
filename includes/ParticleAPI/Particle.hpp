#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <cstdlib>
#include <list>
#include "../Shader.h"
#include "../OpenGLAPI/OpenGLAPI.hpp"

////////////////////////////////////////////////////////////////////////
enum class RENDER_TYPE {RECTANGLE, CIRCLE, TRIANGLE};

////////////////////////////////////////////////////////////////////////
enum class COLOR {RED,BLUE,GREEN,YELLOW,PURPLE};

namespace ParticleAPI{

    ////////////////////////////////////////////////////////////////////////
    class Transform{
        public:
        Transform(float x = 0, float y = 0);

        float getX();

        float getY();

        void move(float x, float y);

        ~Transform();

        private:
        float x,y;
    };

    ////////////////////////////////////////////////////////////////////////
    class MoveableObject{
        public:

        MoveableObject(float X = 0., float Y = 0.,float velX = 0, float velY = 0, float accX = 0,float accY = 0, float mass = 1);

        void move(float time);

        void move(float x, float y);

        void addVel(float time);

        void addForce(float forceX,float forceY);

        float getX();

        float getY();

        virtual ~MoveableObject();

        static int count;

        private:
        float VelX, VelY;
        float AccX, AccY;
        float Mass;
        Transform * transform;
    };

    //Particle object class
    class ParticleObject : protected MoveableObject{
        public:
        ParticleObject(float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width);

        ~ParticleObject();

        bool shoudlDie();

        virtual void Draw(int ResolutionX, int ResolutionY);

        virtual void Move(float deltaTime) = 0;

        virtual ParticleObject* Spawn(float x, float y) = 0;

        bool operator == (ParticleObject* other);

        static int Pcount, PDcount;

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
        FireParticle(float fadeIn,float fadeOut,float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width);

        void Draw (int ResolutionX, int ResolutionY) override;

        void Move(float deltaTime) override;

        ParticleObject* Spawn(float x, float y) override;

        private:

        float FadeIn, FadeOut;

    };

     //Class that receives a particle and spawns particles of that same type
    class ParticleSpawner{
        public:
        ParticleSpawner(float x, float y, float quantity, float timeToLive, ParticleObject* particle);

        virtual void Update(float deltaTime);

        virtual void Draw(int ResolutionX,int ResolutionY);

        bool checkTimeToLive();

        virtual void Spawn();

        virtual ~ParticleSpawner();

        void print();

        bool operator == (ParticleSpawner * other);

        static int Dcount;

        private:
        int ID;
        int Width,Height;
        float Angle, TimeToLive, SpawnTime, LastSpawn, x, y, Quantity;
        ParticleObject* Particle;
        std::list<ParticleObject*> list;
        std::list<ParticleObject*>::iterator it;
    };


    class ParticleManager{
        public:

        static ParticleManager * getParticleManager();

        //Function that is called everytime a ParticleSpawner is created 
        int Insert(ParticleSpawner* spawner);

        //Function that is called everytime a ParticleSpawner is deleted
        void deleteSpawner(std::list<ParticleSpawner*>::iterator& spawner);

        //Function that is called every frame in order to update position
        void Update(float deltaTime);

        //Function that is called every frame in order to draw the particles
        void Draw(int ResolutionX,int ResolutionY);

        static int SpawnerCount;

        private:
        int quantity;
        std::list<ParticleSpawner*> list;
        static ParticleManager * manager;

        ParticleManager();
    };
}
#endif