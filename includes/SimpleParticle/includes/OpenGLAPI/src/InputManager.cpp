#include "InputManager.hpp"

using namespace OpenGLAPI;
using namespace Manager;
//Classe responsável por manejar input/informações externas ao programa
InputManager * InputManager::manager = nullptr;

InputManager * InputManager::getInputManager(){
    if(!manager) manager = new InputManager();
    return manager;}


void InputManager::setKey(void (*keyFunc)(int), int key){
    keyArray[key] = keyFunc;}


void InputManager::Update(){
    getMouseValue();}


double * InputManager::getMouse(){
    return mouse;}

InputManager::InputManager(){
    //Setting up for key callback functions
    keyArray = (void(**)(int)) malloc(sizeof(void(**)(int))*348);
    for(int i = 0; i < 348; i ++) keyArray[i] = NULL;
    int index = glfwGetKeyScancode(GLFW_KEY_ESCAPE);
    glfwSetKeyCallback(OpenGLAPI::window, (void (*)(GLFWwindow*, int , int, int, int))&keyCallbackFunction);
    void (*escapeFunc)(int) = [](int){glfwSetWindowShouldClose(OpenGLAPI::window, true);};
    setKey(escapeFunc, index);}


void InputManager::getMouseValue(){
    glfwGetCursorPos(OpenGLAPI::window,&mouse[0], &mouse[1]);
    mouse[2] = 0.0;
    mouse[3] = 0.0;
    if (glfwGetMouseButton(OpenGLAPI::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mouse[2] = 1.0; 
    if (glfwGetMouseButton(OpenGLAPI::window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mouse[3] = 1.0;

    //Normalizando as coordenadas do cursor
    int width, height;
    glfwGetFramebufferSize(OpenGLAPI::window, &width, &height);

    mouse[0] = (((mouse[0]*2.0)/width) -1.0)*-1;
    mouse[1] = (((mouse[1]*2.0)/height) -1.0)*-1;}

    GLFWkeyfun keyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods){
        if(OpenGLAPI::Manager::InputManager::getInputManager()->keyArray[scancode] != NULL){
                InputManager::getInputManager()->keyArray[scancode](scancode);
            }
        }