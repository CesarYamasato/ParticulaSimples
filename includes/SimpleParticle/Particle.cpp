#include "Particle.hpp"


using namespace ParticleAPI;

    //////////////////////////////////////////////////////////////////////////////////////////////
    //TRANSFORM                                                                                 //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int Transform::numDeletes = 0;
    int Transform::numCreates = 0;
    Transform::Transform(float x, float y){
        this->x = x;
        this->y = y;
        numCreates++;
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
        numDeletes++;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //MOVEABLE OBJECT                                                                           //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int MoveableObject::count = 0;

    //TODO: MUDAR FORMA COMO É CALCULADA VELOCIDADE E ACELERAÇÃO (RETIRAR TIME DOS PARÂMETROS OU ESCONDER DO USUÁRIO)
    //REVAMP NO SISTEMA DE UPDATE 

    MoveableObject::MoveableObject(float X, float Y,float velX, float velY , float accX ,float accY , float mass){
        transform = new ParticleAPI::Transform(X,Y);
        VelX = velX;
        VelY = velY;
        AccX = accX;
        AccY = accY;
        Mass = mass;
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
    ParticleObject::ParticleObject(float x, float y, float timeToLive, Shader * shader, OpenGLAPI::Texture * texture, int height, int width):
    MoveableObject(x,y)
    {   //TODO: ARRUMAR NOME DAS VARIÁVEIS
        float time = glfwGetTime();
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
        float time = glfwGetTime();
        bool shoulddie = (SpawnTime + TimeToLive) < time;
        return shoulddie;
    }

    void ParticleObject::Draw() {
        float x = transform->getX();
        float y = transform->getY();
        Renderer->draw(x,y,Width,Height, 1.0, Texture);
    }

    bool ParticleObject::operator == (ParticleObject* other){
        return this->ID == other->ID;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////
    //FIRE PARTICLE                                                                             //
    //////////////////////////////////////////////////////////////////////////////////////////////


    FireParticle::FireParticle(float x, float y,float fadeIn,float fadeOut,float timeToLive, float windHeight, float wind, Shader * shader, OpenGLAPI::Texture * texture, int height, int width)
    : ParticleObject(x, y, timeToLive, shader, texture, height, width)
    { //TODO: ARRUMAR NOME DAS VARIÁVEIS
        FadeIn = fadeIn;
        FadeOut = fadeOut;
        this->wind = wind;
        this->windHeight = windHeight;
        this->seededTime = SpawnTime + rand()%30;
    }

    void FireParticle::Draw (){
        float x = transform->getX();
        float y = transform->getY();
        float opacity = 1.0;
        float currentTime = glfwGetTime();
        if(currentTime >= SpawnTime+TimeToLive-FadeOut) opacity = 1.0-((currentTime - (SpawnTime + TimeToLive - FadeOut))/FadeOut);
        this->shader->use();
        this->shader->setFloat("time", seededTime);
        Renderer->draw(x,y,Width,Height, opacity, Texture);
    }

    void FireParticle::Move(float deltaTime){
        //int time = static_cast<int>(OpenGLAPI::InputManager::getInputManager()->getTime());
        int random = rand()%2;
        int signal = (rand()%2)*2 - 1;
        float factor = rand()%2+(rand()%9)/10;
        float height = this->transform->getY();
        float windFactor = this->transform->getY() >= windHeight ? 1: (height/windHeight)*0.7;
        MoveableObject::move((windFactor*wind+random*factor*15*Width*signal)*deltaTime, Height*deltaTime*2);
        seededTime += deltaTime;
    }

    ParticleObject* FireParticle::Spawn(float x, float y){
        ParticleObject::Pcount++;
        ParticleObject* particle = new FireParticle(x, y, FadeIn, FadeOut, TimeToLive, windHeight, wind, shader, Texture, Height, Width);
        return particle;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////
    //PARTICLE SPAWNER                                                                          //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int ParticleSpawner::Dcount = 0;
    //Class that receives a particle and spawns particles of that same type
    ParticleSpawner::ParticleSpawner(float x, float y, float quantity, float timeToLive, ParticleObject* particle){
        ID = ParticleManager::getParticleManager()->Insert(this);
        float time = glfwGetTime();
        TimeToLive = timeToLive;
        SpawnTime = time;
        Particle = particle;
        pos = new Transform(x,y);
        Quantity = quantity;
        Dcount++;
    }

    void ParticleSpawner::Update(float deltaTime){
        for(it = list.begin(); it != list.end(); ++it){
            if((*it)->shoudlDie()){
                delete(*it);
                list.erase(it++);
                continue;
            }
            (*it)->Move(deltaTime);
        }
    }

    void ParticleSpawner::Draw(){
        for(it = list.begin(); it != list.end(); ++it){
            (*it)->Draw();
        }
    }

    bool ParticleSpawner::checkTimeToLive(){
        float time = glfwGetTime();
        return ((SpawnTime + TimeToLive) < time);
    }

    void ParticleSpawner::Spawn(){
        float time = glfwGetTime();
        if(time > LastSpawn + Quantity) {
            list.push_front(Particle->Spawn(pos->getX(),pos->getY()));
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
        }
        list.clear();
    }

    bool ParticleSpawner::operator == (ParticleSpawner * other){
        bool returnVal = (this->ID == other->ID);
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
                (*it)->Draw();
            }
            else break;
        }
    }

    //Function that is called every frame in order to draw the particles
    void ParticleManager::Draw(){
        std::list<ParticleSpawner*>::iterator it;
        for(it = list.begin(); it != list.end(); ++it){
            (*it)->Draw();
        }
    }

    ParticleManager::~ParticleManager(){
        std::list<ParticleSpawner*>::iterator it;
        for(it = list.begin(); it != list.end(); ++it){
            if(quantity){
                deleteSpawner(it);
            }
        }
    }

    ParticleManager::ParticleManager(){
        quantity = 0;
    }