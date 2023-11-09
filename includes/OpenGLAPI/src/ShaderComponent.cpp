#include "ShaderComponent.hpp"

using namespace OpenGLAPI;
using namespace ShaderComponent;

Component::Component(Shader * shader): shader(shader){}
Component::Component(const char * vertex_path, const char * fragment_path):shader(new Shader(vertex_path, fragment_path)){}

void ConcreteComponent::Draw() const{

}

Decorator(Component* shaderComponent, Shader * shader): component(shaderComponent), ShaderComponent(shader){}



