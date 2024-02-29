#version 460 core
#define PI 3.141592653589793 

uniform sampler2D Textura;
out vec4 color;
in vec2 Textpos;
in float Opacity;
in vec2 Resolution;
in vec2 Center;
in vec2 Size;
in float Time;

float screenArea = Resolution.x*Resolution.y; //screen area in pixels
float size = (Size.x*Size.y); //particle size relative to screen size
float pixelParticleArea = screenArea/size;
vec2 pos = gl_FragCoord.xy/Resolution;
vec2 center = Center/Resolution;

mat2 translateVec2(vec2 vec){
    mat2 translation = mat2(
        Resolution.x/vec.x, 0.,
        0., Resolution.y/vec.y
    );
    return translation;
}

float Dimming(float radius, float ring){
    mat2 translate = translateVec2(Size);
    float dist = distance(translate*pos, translate*vec2(1.-center.x, center.y));
    float factor = dist > radius? ((0.8-dist*1.5)*(cos(Time*3)+1)/2) : (cos(dist*((PI)/radius))+3)/4;
    factor = dist > ring? 0. : factor;
    return factor;
}

void main(){
    vec3 col = vec3(1.0,0.5,0.0);
    float factor = Dimming(0.15, 0.9);
    color = vec4(col,factor*Opacity);
}
