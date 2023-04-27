
#include "OpenGLAPI.hpp"

using namespace OpenGLAPI;

    int OpenGLAPI::libInit(int SCR_WIDTH, int SCR_HEIGHT, const char* name, void framebuffer_size_callback(GLFWwindow* window, int width, int height)){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        OpenGLAPI::window  = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return 0;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
        glfwSwapInterval(1);
        return 1;
    }

    void* OpenGLAPI::initGlad(const char * name){
        return (void*) glfwGetProcAddress(name);
    }

    void OpenGLAPI::render(GLFWwindow * window){
        glfwSwapBuffers(OpenGLAPI::window);
        glfwPollEvents();
    }

    int OpenGLAPI::WindowShouldClose(GLFWwindow * window){
        return glfwWindowShouldClose(window);
    }

    void OpenGLAPI::setwindowTitle(GLFWwindow * window, const char* title){
        glfwSetWindowTitle(window,title);
    }

    void OpenGLAPI::Terminate(){
        glfwTerminate();
    }

    void OpenGLAPI::setDefaultParams(Shader shader){
            InputManager * manager = InputManager::getInputManager();
            shader.setVec4("Mouse", manager->getMouse());
            shader.setFloat("Time", manager->getTime());
            shader.setVec2("Resolution", manager->getResolution());
    }

    //Função responsável por encontrar o path para um determinado diretório a partir do diretório pai
    const char* OpenGLAPI::GetPathTo(const std::string directory){
        std::filesystem::path path = std::filesystem::current_path();
        std::string * parentPath = new std::string(path.parent_path().string());
        std::string * finalPath = new std::string (*parentPath + directory);
        const char * returnPath = ((*finalPath).c_str());
        return returnPath;
    }

    //Função responsável por mensagens de debug
    void APIENTRY OpenGLAPI::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
    {
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
    switch (source)
        {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //InputManager                                                                              //
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Classe responsável por manejar input/informações externas ao programa
        InputManager* InputManager::manager = nullptr;
        InputManager * InputManager::getInputManager()
        {
         if (!InputManager::manager) InputManager::manager = new InputManager;
         return InputManager::manager;
        }

        void InputManager::setWindow (GLFWwindow *window){
            Window = window;
        }

        void InputManager::setKeyCallBackFunction(){
            glfwSetKeyCallback(Window, (void (*)(GLFWwindow*, int , int, int, int))keyCallback);
        }

        void InputManager::setKey(void (*keyFunc)(int), int scanCode){
            keyArray[scanCode] = keyFunc;
        }

        void InputManager::processInput(){
            time = (float) glfwGetTime();
            if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(Window, true);
            getMouseValue();
            getKeysPressed(Window);
            //getInputArray();
            //printArray();
        }

        double * InputManager::getResolution(){
            int height, width;
            glfwGetFramebufferSize(Window, &width, &height);
            resolution[0] = width;
            resolution[1] = height;
            return resolution;
        }

        float InputManager::getTime(){
            time = glfwGetTime();
            return time;
        }

        double * InputManager::getMouse(){
            getMouseValue();
            return mouse;
        }

        InputManager:: InputManager()
        {
            mouse = (double*) malloc(sizeof(double)*4);
            shouldDraw = true;
            press = false;
            float time = (float) glfwGetTime();
            resolution = (double *) malloc (sizeof(int)*2);

            //
            Window = (GLFWwindow*) malloc(sizeof(GLFWwindow*));

            //Setting up for key callback functions
            keyArray = (void(**)(int)) malloc(sizeof(void(**)(int))*348);
            for(int i = 0; i < 348; i ++) keyArray[i] = (void(*)(int)) malloc(sizeof(void(*)(int)));
            for(int i = 0; i < 348; i ++) keyArray[i] = NULL;
            keyCallback = &OpenGLAPI::InputManager::keyCallbackFunction;
        }

        void InputManager::getMouseValue(){
            glfwGetCursorPos(Window,&mouse[0], &mouse[1]);
            mouse[2] = 0.0;
            mouse[3] = 0.0;
            if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mouse[2] = 1.0; 
            if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mouse[3] = 1.0;

            //Normalizando as coordenadas do cursorint height, width;
            int width, height;
            glfwGetFramebufferSize(Window, &width, &height);
            resolution[0] = width;
            resolution[1] = height;
            mouse[0] = ((mouse[0]*2.0)/resolution[0]) -1.0;
            mouse[1] = ((mouse[1]*2.0)/resolution[1]) -1.0;
        }

        void InputManager::getKeysPressed(GLFWwindow *window){ 
            if(glfwGetKey(window, 32) == GLFW_PRESS && !press) {
                shouldDraw = !shouldDraw;
                press = true;
            }
            if(glfwGetKey(window, 32) == GLFW_RELEASE) press = false;
        }

        /*void InputManager::getInputArray(){
            static void * array = nullptr;
            if(!array)array = malloc(sizeof(double*)*6 + sizeof(bool)*2 + sizeof(float));
            *(bool*) array = shouldDraw;
            *(bool*) (array + sizeof(bool)) = press;
            for(int i = 0; i < 4; i++) *(double *) (array + sizeof(double)*i + 2*sizeof(bool)) = mouse[i];
            *(double *) (array + sizeof(double)*4 + 2*sizeof(bool)) = (double) height;
            *(double *) (array + sizeof(double)*5 + 2*sizeof(bool)) = (double) width;
            *(float*) (array + sizeof(double)*6 + sizeof(bool)*2) = time;  
            this->array = (void*) array;
        }*/

        void InputManager::keyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods){
            if(OpenGLAPI::InputManager::getInputManager()->keyArray[key] != NULL)OpenGLAPI::InputManager::getInputManager()->keyArray[key](scancode);
        }


    ////////////////////////////////////////////////////////////////////////////////////////////// 
    //DebugManager                                                                              //
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Classe responsável por ativar e desativar mensagens de debug

        DebugManager * DebugManager::getDebugManager(){
            static DebugManager * manager = nullptr;
            if(!manager) manager = new DebugManager;
            return manager;
        }

        void DebugManager::DisableDebug(){
            glDisable(GL_DEBUG_OUTPUT);
        }

        void DebugManager::EnableDebug(){
            glEnable(GL_DEBUG_OUTPUT);
        }

        DebugManager::DebugManager(){
        glEnable(GL_DEBUG_OUTPUT );
        glDebugMessageCallback(OpenGLAPI::MessageCallback, 0 );
        }


    //////////////////////////////////////////////////////////////////////////////////////////////
    //TexturaClass                                                                              //
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Classe para texturas
        Texture::Texture(unsigned int Height,unsigned int Width, GLenum WrapParams_S, GLenum WrapParams_T, GLenum MagParams, GLenum MinParams, unsigned char* data)
        {  

            glGenTextures(1,&textureID);
            glBindTexture(GL_TEXTURE_2D,textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapParams_S);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapParams_T);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinParams);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagParams);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            this->Height = Height;
            this->Width = Width;

            glBindTexture(GL_TEXTURE_2D, 0);

            Data = data;
        }
        
        Texture::operator GLuint(){
            return textureID;
        }

        void Texture::bind(){
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        unsigned int Texture::getID(){
            return textureID;
        }

        unsigned int Texture::getHeight(){
            return Height;
        }

        unsigned int Texture::getWidth(){
            return Width;
        }

        Texture::~Texture(){
            delete(Data);
        }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //SpriteRenderer                                                                            //
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Classe para reenderizar sprites
        int SpriteRenderer::NumRenderer = 0;
        int SpriteRenderer::DelRenderer = 0;
        SpriteRenderer::SpriteRenderer(Shader * shader){
            this->shader = shader;
            glGenVertexArrays(1,&quadVAO);
            glGenBuffers(1, &VBO);
            NumRenderer++;
        }

        void SpriteRenderer::draw(OpenGLAPI::Texture* texture,int resolutionX, int resolutionY, float x, float y,float sizex,float sizey, float opacity){
            initRenderData();
            this->shader->use();
            glm::mat4 model = glm::mat4(1.f);

            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(resolutionX), 
            static_cast<float>(resolutionY), 0.0f, -1.0f, 1.0f);

            model = glm::translate(model,glm::vec3(resolutionX - x,resolutionY - y, 0.0f));
            model = glm::scale(model, glm::vec3(sizex, sizey, 1.0f));

            this->shader->setMat4("model", model);
            this->shader->setMat4("projection", projection);
            this->shader->setFloat("Opacity", opacity);

            glActiveTexture(GL_TEXTURE0);
            texture->bind();

            this->shader->setSampler("Textura", 0);

            glBindVertexArray(this->quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

        }

        void SpriteRenderer::initRenderData(){
            // configure VAO/VBO
            float vertices[] = { 
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 
            
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->quadVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }

        SpriteRenderer::~SpriteRenderer(){
           DelRenderer++;
        }