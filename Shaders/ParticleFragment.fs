#version 460 core

uniform sampler2D Textura;
out vec4 color;
in vec2 Textpos;
in float Opacity;
in vec2 Resolution;
in vec2 Center;
in vec2 Size;
in float Time;

float screenArea = Resolution.x*Resolution.y; //screen area in pixels
vec2 size = Size/Resolution; //particle size relative to screen size
float pixelParticleArea = (Size.x*Size.y)/screenArea;
vec2 pos = gl_FragCoord.xy/Resolution;
vec2 center = Center/Resolution;

mat2 translateVec2(vec2 vec){
    mat2 translation = mat2(
        Resolution.x/vec.x, 0.,
        0., Resolution.y/vec.y
    );
    return translation;
}

float Dimming(){
    mat2 translate = translateVec2(Size);
    float dist = distance(translate * pos, translate * vec2(1.-center.x, center.y));
    float factor = dist > 0.15? ((0.8-dist)*(cos(Time*3)+1)/2) : 1.;
    return factor;
}

void main(){
    vec3 col = vec3(1.0,0.5,0.0);
    float factor = Dimming();
    color = vec4(col,factor*Opacity);
}
