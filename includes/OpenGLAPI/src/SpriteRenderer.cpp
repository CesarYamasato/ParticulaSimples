#include "SpriteRenderer.hpp"

using namespace OpenGLAPI;
using namespace Renderer;

//////////////////////////////////////////////////////////////////////////////////////////////
    //SpriteRenderer                                                                            //
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Classe para reenderizar sprites
        int SpriteRenderer::NumRenderer = 0;
        int SpriteRenderer::DelRenderer = 0;

        // SpriteRenderer::SpriteRenderer(){
        //     this->shader = shader;
        //     glGenVertexArrays(1,&quadVAO);
        //     glGenBuffers(1, &VBO);
        //     NumRenderer++;
        // }

        SpriteRenderer::SpriteRenderer(Shader * shader){
            this->shader = shader;
            glGenVertexArrays(1,&quadVAO);
            glGenBuffers(1, &VBO);
            initRenderData();
            NumRenderer++;
        }

        void SpriteRenderer::draw(float x, float y,float sizex,float sizey, float opacity, OpenGLAPI::Texture* texture){
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

            glBindVertexArray(this->quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }

        SpriteRenderer::~SpriteRenderer(){
            DelRenderer++;
        }