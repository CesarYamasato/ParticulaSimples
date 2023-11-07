#version 460 core

uniform sampler2D Textura;
out vec4 color;
in vec2 Textpos;
in float Opacity;
in vec2 Resolution;
in vec2 Center;
in vec2 Size;

float screenArea = Resolution.x*Resolution.y; //screen area in pixels
vec2 size = Size/Resolution; //particle size relative to screen size
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
    float factor = 1.;
    if(dist > 0.01*length(size)) {factor =1.-dist*1.3;}
    return factor;
}

void main(){
    vec3 col = vec3(1.0,0.5,0.0);
    float factor = Dimming();
    //float dist = 1.-distance(pos, vec2(1.-center.x, center.y));
    color = vec4(col,factor*Opacity);
}
