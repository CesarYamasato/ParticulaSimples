#ifndef OPENGLAPI_SPRITERENDERER
#define OPENGLAPI_SPRITERENDERER

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "common.hpp"
#include "../../Shader.h"
#include <iostream>

void keyCallbackFunction(GLFWwindow*, int, int, int, int);

namespace OpenGLAPI{ namespace Renderer{
    //Classe para reenderizar sprites
    class SpriteRenderer{
            public:
            SpriteRenderer();

            SpriteRenderer(Shader * shader);

            void draw(float x, float y,float sizex,float sizey, float opacity, OpenGLAPI::Texture* texture = nullptr);

            ~SpriteRenderer();
            
            Shader * shader;
            Shader * spriteShader;
            
            static int NumRenderer, DelRenderer;

            private:
            unsigned int quadVAO;
            unsigned int VBO;
            double size[2];
            double center[2];

            void initRenderData();
        };
    };
} //namespace OpenGLAPI::SpriteRenderer
#endif