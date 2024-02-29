#include "ShaderComponent.hpp"

using namespace OpenGLAPI;
using namespace ShaderComponent;

Component::Component(Shader * shader): shader(shader){}
Component::Component(const char * vertex_path, const char * fragment_path):shader(new Shader(vertex_path, fragment_path)){}

void ConcreteComponent::Draw(float x, float y,float sizex,float sizey) const{

}

Decorator::Decorator(Component* shaderComponent, Shader * shader): component(shaderComponent), Component(shader){}
Decorator::Decorator(Component* shaderComponent, const char * vertex_path, const char * fragment_path): component(shaderComponent),
                                                                                            Component(vertex_path, fragment_path){}



