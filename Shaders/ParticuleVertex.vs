#version 460 core
layout (location = 0) in vec2 aPos;
uniform vec2 Resolution;
uniform vec2 TextResolution;
uniform float Opacity;
out float opacity;
out vec2 pos;
void main(){
    pos = vec2(aPos.x,-aPos.y)/Resolution;
    vec2 finalPos = ((aPos/Resolution)*2.0)-1.0;
    gl_Position = vec4(finalPos,0.0, 1.0);
    opacity = Opacity;
}
