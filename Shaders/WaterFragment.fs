#version 460 core

uniform sampler2D Textura;
out vec4 color;
in vec2 pos;
in float opacity;
void main(){
    color = vec4(0.0,0.0,1.0,0.);
}
