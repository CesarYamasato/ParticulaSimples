#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <cstdlib>
#include <list>
#include "../Shader.h"
#include "../OpenGLAPI/src/OpenGLAPI.hpp"


namespace ParticleAPI{

    ////////////////////////////////////////////////////////////////////////
    class Transform{
        public:
        Transform(float x = 0, float y = 0);

        float getX();

        float getY();

        void move(float x, float y);

        ~Transform();

        static int numDeletes, numCreates;

        private:
        float x,y;
    };

    ////////////////////////////////////////////////////////////////////////
    class MoveableObject{
        public:
        Transform * transform;

        MoveableObject(float X, float Y,float velX = 0, float velY = 0, float accX = 0,float accY = 0, float mass = 1);

        void move(float time);

        void move(float x, float y);

        void addVel(float time);

        void addForce(float forceX,float forceY);

        virtual ~MoveableObject();

        static int count;

        private:
        float VelX, VelY;
        float AccX, AccY;
        float Mass;
    };

    //Particle object class
    class ParticleObject : protected MoveableObject{
        public:
        ParticleObject(float x, float y, float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width);

        ~ParticleObject();

        bool shoudlDie();

        virtual void Draw();

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
        FireParticle(float x, float y, float fadeIn,float fadeOut,float timeToLive, float windHeight, float wind, Shader * shader, OpenGLAPI::Texture * texture, int height, int width);

        void Draw () override;

        void Move(float deltaTime) override;

        ParticleObject* Spawn(float x, float y) override;

        private:

        float FadeIn, FadeOut, wind, windHeight;

    };

     //Class that receives a particle and spawns particles of that same type
    class ParticleSpawner{
        public:
        ParticleSpawner(float x, float y, float quantity, float timeToLive, ParticleObject* particle);

        virtual void Update(float deltaTime);

        virtual void Draw();

        bool checkTimeToLive();

        virtual void Spawn();

        virtual ~ParticleSpawner();

        void print();

        bool operator == (ParticleSpawner * other);

        static int Dcount;

        private:
        int ID;
        int Width,Height;
        float Angle, TimeToLive, SpawnTime, LastSpawn, Quantity;
        Transform * pos;
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

        virtual ~ParticleManager();

        static int SpawnerCount;

        private:
        int quantity;
        std::list<ParticleSpawner*> list;
        static ParticleManager * manager;

        //Function that is called every frame in order to draw the particles
        void Draw();

        ParticleManager();
    };
}
#endif