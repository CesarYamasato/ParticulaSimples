
#include "OpenGLAPI.hpp"

using namespace OpenGLAPI;

    GLFWwindow * OpenGLAPI::window;

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

    void OpenGLAPI::setwindowTitle(const char* title){
        glfwSetWindowTitle(window,title);
    }

    void OpenGLAPI::Terminate(){
        glfwTerminate();
    }

    void OpenGLAPI::setDefaultParams(Shader shader){
            int width,height;
            double * resolution = (double *) malloc(sizeof(double)*2);
            glfwGetFramebufferSize(OpenGLAPI::window,&width,&height);
            Manager::InputManager * manager = Manager::InputManager::getInputManager();
            shader.setVec4("Mouse", manager->getMouse());
            shader.setFloat("Time", glfwGetTime());
            shader.setVec2("Resolution", resolution);
            delete(resolution);
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

    //////////////////////////////////////////////////////////////////////////////////////////////
    //SpriteRenderer                                                                            //
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Classe para reenderizar sprites
        int SpriteRenderer::NumRenderer = 0;
        int SpriteRenderer::DelRenderer = 0;

        SpriteRenderer::SpriteRenderer(){
            this->shader = shader;
            glGenVertexArrays(1,&quadVAO);
            glGenBuffers(1, &VBO);
            NumRenderer++;
        }

        SpriteRenderer::SpriteRenderer(Shader * shader){
            this->shader = shader;
            glGenVertexArrays(1,&quadVAO);
            glGenBuffers(1, &VBO);
            NumRenderer++;
        }

        void SpriteRenderer::draw(float x, float y,float sizex,float sizey, float opacity, OpenGLAPI::Texture* texture){
            initRenderData();
            this->shader->use();
            glm::mat4 model = glm::mat4(1.f);

            int resolutionX, resolutionY;

            glfwGetFramebufferSize(OpenGLAPI::window,&resolutionX, &resolutionY);

            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(resolutionX), static_cast<float>(resolutionY), 0.0f, -1.0f, 1.0f);
            model = glm::translate(model,glm::vec3(resolutionX - x,resolutionY - y, 0.0f));
            model = glm::scale(model, glm::vec3(sizex, sizey, 1.0f));

            double res[2] = {static_cast<float>(resolutionX), static_cast<float>(resolutionY)};
            double center[2] = {x-sizex/2, y-sizey/2};
            double size[2] = {sizex, sizey};

            this->shader->setMat4("model", model);
            this->shader->setMat4("projection", projection);
            this->shader->setFloat("opacity", opacity);
            this->shader->setVec2("resolution", res);
            this->shader->setVec2("center", center);
            this->shader->setVec2("size", size);

            if(texture != nullptr){
                glActiveTexture(GL_TEXTURE0);
                texture->bind();
                this->shader->setSampler("Textura", 0);}

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