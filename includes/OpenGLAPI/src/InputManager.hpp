#ifndef OPENGLAPI_INPUTMANAGER
#define OPENGLAPI_INPUTMANAGER

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "../common.hpp"
#include <iostream>

GLFWkeyfun keyCallbackFunction(GLFWwindow*, int, int, int, int);

namespace OpenGLAPI{ namespace Manager{
    //Classe responsável por manejar input/informações externas ao programa
    class InputManager{
        public:

        static InputManager * getInputManager();


        void setKey(void (*keyFunc)(int), int scanCode);


        void Update();


        double * getMouse();

        InputManager & operator = (const InputManager &) = delete;

        void (**keyArray)(int);
        
        private:

        static InputManager * manager;

        double mouse[4];

        InputManager();


        void getMouseValue();

        ~InputManager() {}
    };
}} //namespace OpenGLAPI::Manager
#endif