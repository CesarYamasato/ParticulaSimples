#version 460 core

uniform sampler2D Textura;
out vec4 color;
in vec2 Textpos;
in float Opacity;
in vec2 Center;
in vec2 Size;

void main(){
    vec3 col = vec3(1.0,0.5,0.0);
    vec3 temp = vec3(1.0);
    color = vec4(texture(Textura, Textpos).xyz,Opacity);
}
