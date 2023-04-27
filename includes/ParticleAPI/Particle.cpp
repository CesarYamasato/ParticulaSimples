#include "Particle.hpp"


using namespace ParticleAPI;

    //////////////////////////////////////////////////////////////////////////////////////////////
    //TRANSFORM                                                                                 //
    //////////////////////////////////////////////////////////////////////////////////////////////

    Transform::Transform(float x, float y){
        this->x = x;
        this->y = y;
    }

    float Transform::getX(){
        return x;
    }

    float Transform::getY(){
        return y;
    }

    void Transform::move(float x, float y){
        this->x += x;
        this->y += y;
    }

    Transform::~Transform(){
        
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //MOVEABLE OBJECT                                                                           //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int MoveableObject::count = 0;

    MoveableObject::MoveableObject(float X, float Y,float velX, float velY, float accX,float accY, float mass){
        transform = new ParticleAPI::Transform(X,Y);
        VelX = velX;
        VelY = velY;
        AccX = accX;
        AccY = accY;
        Mass = mass;
        //std::cout << "TESTE" << std::endl;
    }

    void MoveableObject::move(float time){
        transform->move(VelX*time,VelY*time);
    }

    void MoveableObject::move(float x, float y){
        transform->move(x,y);
    }

    void MoveableObject::addVel(float time){
        VelX += AccX*time;
        VelY += AccY*time;
    }

    void MoveableObject::addForce(float forceX,float forceY){
        AccX += forceX/Mass;
        AccY += forceY/Mass;
    }

    float MoveableObject::getX(){
        return transform->getX();
    }

    float MoveableObject::getY(){
        return transform->getY();
    }

    MoveableObject::~MoveableObject(){
        delete(transform);
        count ++;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //BASE PARTICLE                                                                             //
    //////////////////////////////////////////////////////////////////////////////////////////////

    //Particle object class
    int ParticleObject::Pcount = 0;
    int ParticleObject::PDcount = 0;
    ParticleObject::ParticleObject(float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width):
    MoveableObject()
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

    ParticleObject::~ParticleObject(){
        delete (Renderer);
        PDcount++;
    }

    bool ParticleObject::shoudlDie(){
        float time = OpenGLAPI::InputManager::getInputManager()->getTime();
        bool shoulddie = (SpawnTime + TimeToLive) < time;
        return shoulddie;
    }

    void ParticleObject::Draw(int ResolutionX, int ResolutionY) {
        float x = getX();
        float y = getY();
        Renderer->draw(Texture,ResolutionX, ResolutionY,x,y,Width,Height, 1.0);
    }

    bool ParticleObject::operator == (ParticleObject* other){
        return this->ID == other->ID;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////
    //FIRE PARTICLE                                                                             //
    //////////////////////////////////////////////////////////////////////////////////////////////


    FireParticle::FireParticle(float fadeIn,float fadeOut,float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
    : ParticleObject(timeToLive, shader, texture, height, width)
    {
        FadeIn = fadeIn;
        FadeOut = fadeOut;
    }

    void FireParticle::Draw (int ResolutionX, int ResolutionY){
        float x = getX();
        float y = getY();
        float opacity = 1.0;
        float currentTime = OpenGLAPI::InputManager::getInputManager()->getTime();
        if(currentTime <= FadeIn+SpawnTime) opacity = 1.0-((FadeIn + SpawnTime -currentTime)/FadeIn);
        else if(currentTime >= SpawnTime+TimeToLive-FadeOut) opacity = 1.0-((currentTime - (SpawnTime + TimeToLive - FadeOut))/FadeOut);

        Renderer->draw(Texture,ResolutionX, ResolutionY,x,y,Width,Height, opacity);
    }

    void FireParticle::Move(float deltaTime){
        //int time = static_cast<int>(OpenGLAPI::InputManager::getInputManager()->getTime());
        int random = rand()%2;
        int signal = (rand()%2) - 1;
        if(signal == 0) signal = 1;
        MoveableObject::move(random*10*Width*signal*deltaTime, Height*deltaTime*2);
    }

    ParticleObject* FireParticle::Spawn(float x, float y){
        ParticleObject::Pcount++;
        ParticleObject* particle = new FireParticle(FadeIn, FadeOut, TimeToLive, shader, Texture, Height, Width);
        particle->MoveableObject::move(x,y);
        return particle;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////
    //PARTICLE SPAWNER                                                                          //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int ParticleSpawner::Dcount = 0;
    //Class that receives a particle and spawns particles of that same type
    ParticleSpawner::ParticleSpawner(float x, float y, float quantity, float timeToLive, ParticleObject* particle){
        ID = ParticleManager::getParticleManager()->Insert(this);
        float time = OpenGLAPI::InputManager::getInputManager()->getTime();
        TimeToLive = timeToLive;
        SpawnTime = time;
        Particle = particle;
        this->x = x;
        this->y = y;
        Quantity = quantity;
        Dcount++;
    }

    void ParticleSpawner::Update(float deltaTime){
        for(it = list.begin(); it != list.end(); ++it){
            if((*it)->shoudlDie()){
                delete(*it);
                list.erase(it++);
                //Dcount++;
                continue;
            }
            (*it)->Move(deltaTime);
        }
    }

    void ParticleSpawner::Draw(int ResolutionX,int ResolutionY){
        for(it = list.begin(); it != list.end(); ++it){
            (*it)->Draw(ResolutionX, ResolutionY);
        }
    }

    bool ParticleSpawner::checkTimeToLive(){
        float time = OpenGLAPI::InputManager::getInputManager()->getTime();
        return ((SpawnTime + TimeToLive) < time);
    }

    void ParticleSpawner::Spawn(){
        float time = OpenGLAPI::InputManager::getInputManager()->getTime();
        if(time > LastSpawn + Quantity) {
            list.push_front(Particle->Spawn(x,y));
            LastSpawn = time;
        }
    }

    void ParticleSpawner::print(){
        std::cout << "ID: " << ID << std::endl;
    }

    ParticleSpawner::~ParticleSpawner(){
        while(!list.empty()) {
            delete list.front();
            list.pop_front();
            std::cout << "DELETADO PRA KARALHO" << std::endl;
         }
        list.clear();
    }

    bool ParticleSpawner::operator == (ParticleSpawner * other){
        bool returnVal = (this->ID == other->ID);
        std::cout << "CONFERIDO IRMÃO" << std::endl;
        return returnVal;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //PARTICLE MANAGER                                                                          //
    //////////////////////////////////////////////////////////////////////////////////////////////
     ParticleManager * ParticleManager::manager = nullptr;
     int ParticleManager::SpawnerCount = 0;

    ParticleManager * ParticleManager::getParticleManager(){;
        if(!ParticleManager::manager) ParticleManager::manager = new ParticleManager();
        return ParticleManager::manager;
    }

    //Function that is called everytime a ParticleSpawner is created 
    int ParticleManager::Insert(ParticleSpawner* spawner){
        list.insert(list.begin(), spawner);
        quantity++;
        return quantity;
    }

    //Function that is called everytime a ParticleSpawner is deleted
    void ParticleManager::deleteSpawner(std::list<ParticleSpawner*>::iterator& spawner){
        std::cout << "REMOVIDO IRMÃO" << std::endl;
        delete(*spawner);
        list.erase(spawner++);
        SpawnerCount++;
        quantity--;
    }

    //Function that is called every frame in order to update position
    void ParticleManager::Update(float deltaTime){
        std::list<ParticleSpawner*>::iterator it;
        for(it = list.begin(); it != list.end(); ++it){
            if(quantity){
                if((*it)->checkTimeToLive()){ 
                    deleteSpawner(it);
                    continue;
                }
                (*it)->Spawn();
                (*it)->Update(deltaTime);
            }
            else break;
        }
    }

    //Function that is called every frame in order to draw the particles
    void ParticleManager::Draw(int ResolutionX,int ResolutionY){
        std::list<ParticleSpawner*>::iterator it;
        for(it = list.begin(); it != list.end(); ++it){
            (*it)->Draw(ResolutionX,ResolutionY);
        }
    }

   ParticleManager::ParticleManager(){
        manager = nullptr;
        quantity = 0;
    }