#version 460 core
layout (location = 0) in vec4 vertex;
uniform mat4 model;
uniform mat4 projection;
uniform float opacity;
uniform vec2 resolution;
uniform vec2 center;
uniform vec2 size;
uniform float time;
out float Opacity;
out vec2 Textpos;
out vec2 Resolution;
out vec2 Center;
out vec2 Size;
out float Time;

void main(){
    //gl_Position = vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * model * vec4(vertex.xy, 0.0,1.0);
    Opacity = opacity;
    Textpos = vertex.zw;
    Resolution = resolution;
    Center = center;
    Size = size;
    Time = time;
}
