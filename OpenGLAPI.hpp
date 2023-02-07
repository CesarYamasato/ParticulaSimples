#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

void MessageCallback( GLenum source, GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam );

namespace OpenGLAPI{

    ////////////////////////////////////////////////////////////////////////
    unsigned int SquareIndices[] = {
        0,1,3,
        1,2,3
    };

    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };


    //Classe responsável por manejar input/informações externas ao programa
    class InputManager{
        public:
        static InputManager * getInputManager()
        {
         static InputManager * manager = nullptr;
         if (!manager) manager = new InputManager;
         return manager;
        }

        void processInput(GLFWwindow *window){
            time = (float) glfwGetTime();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
            getMouseValue(window);
            getKeysPressed(window);
            getInputArray();
            //printArray();
        }

        void * getInput(){
            return array;
        }

        int * getResolution(){
            int array[] = {width, height};
            return array;
        }

        float getTime(){
            return time;
        }

        private:

        double* mouse;
        bool shouldDraw;
        bool press;
        float time;
        int height;
        int width;


        void* array;

        InputManager()
        {
            mouse = (double*) malloc(sizeof(double)*4);
            shouldDraw = true;
            press = false;
            float time = (float) glfwGetTime();
        }

        void getMouseValue(GLFWwindow *window){
            glfwGetCursorPos(window,&mouse[0], &mouse[1]);
            mouse[2] = 0.0;
            mouse[3] = 0.0;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mouse[2] = 1.0; 
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mouse[3] = 1.0;

            //Normalizando as coordenadas do cursor
            glfwGetFramebufferSize(window, &width, &height);
            mouse[0] = ((mouse[0]*2.0)/width) -1.0;
            mouse[1] = ((mouse[1]*2.0)/height) -1.0;
            //std::cout << mouse[0] << "\n" << mouse[1] << std::endl; 
        }

        void getKeysPressed(GLFWwindow *window){ 
            if(glfwGetKey(window, 32) == GLFW_PRESS && !press) {
                shouldDraw = !shouldDraw;
                press = true;
            }
            if(glfwGetKey(window, 32) == GLFW_RELEASE) press = false;
        }

        void getInputArray(){
            void * array = malloc(sizeof(double*)*6 + sizeof(bool)*2 + sizeof(float));
            *(bool*) array = shouldDraw;
            *(bool*) (array + sizeof(bool)) = press;
            for(int i = 0; i < 4; i++) *(double *) (array + sizeof(double)*i + 2*sizeof(bool)) = mouse[i];
            *(double *) (array + sizeof(double)*4 + 2*sizeof(bool)) = (double) height;
            *(double *) (array + sizeof(double)*5 + 2*sizeof(bool)) = (double) width;
            *(float*) (array + sizeof(double)*6 + sizeof(bool)*2) = time;  
            this->array = (void*) array;
        }
    };


    //Classe responsável por ativar e desativar mensagens de debug

    class DebugManager
    {
        public:

        static DebugManager * getDebugManager(){
            static DebugManager * manager = nullptr;
            if(!manager) manager = new DebugManager;
            return manager;
        }

        void DisableDebug(){
            glDisable(GL_DEBUG_OUTPUT);
        }

        void EnableDebug(){
            glEnable(GL_DEBUG_OUTPUT);
        }

        private:

        DebugManager(){
        glEnable(GL_DEBUG_OUTPUT );
        glDebugMessageCallback(MessageCallback, 0 );
        }
    };


    //Classe para texturas
    class Texture
    {
        public:
        Texture(unsigned int Height,unsigned int Width, GLenum WrapParams_S = GL_REPEAT, GLenum WrapParams_T = GL_REPEAT, GLenum MagParams = GL_LINEAR, GLenum MinParams = GL_LINEAR, unsigned char* data = NULL)
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
        }
        
        operator GLuint(){
            return textureID;
        }

        void bind(){
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        unsigned int getID(){
            return textureID;
        }

        unsigned int getHeight(){
            return Height;
        }

        unsigned int getWidth(){
            return Width;
        }

        private:
        unsigned int textureID;
        unsigned int Height;
        unsigned int Width;
    };

    //Classe para reenderizar sprites

    class SpriteRenderer{
        public:
        SpriteRenderer(Shader * shader){
            this->shader = shader;
            glGenVertexArrays(1,&quadVAO);
        }

        void draw(OpenGLAPI::Texture* texture,int resolutionX, int resolutionY, float x, float y,float sizex,float sizey, float opacity){
            initRenderData();
            this->shader->use();
            glm::mat4 model = glm::mat4(1.f);

            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(resolutionX), 
            static_cast<float>(resolutionY), 0.0f, -1.0f, 1.0f);

            model = glm::translate(model,glm::vec3(resolutionX - x,resolutionY - y, 0.0f));
            model = glm::scale(model, glm::vec3(sizex, sizey, 1.0f));

            this->shader->setMat4("model", model);
            this->shader->setMat4("projection", projection);
            this->shader->setFloat("opacity", opacity);

            glActiveTexture(GL_TEXTURE0);
            texture->bind();

            this->shader->setSampler("Textura", 0);

            glBindVertexArray(this->quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

        }

        private:
        Shader * shader;
        unsigned int quadVAO;

        void initRenderData(){
            // configure VAO/VBO
            unsigned int VBO;
            float vertices[] = { 
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 
            
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &this->quadVAO);
            glGenBuffers(1, &VBO);
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->quadVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }
    };
}

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
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