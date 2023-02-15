#version 460 core
layout (location = 0) in vec4 vertex;
uniform mat4 model;
uniform mat4 projection;
uniform float Opacity;
out float opacity;
out vec2 pos;
void main(){
    pos = vertex.zw;
    //gl_Position = vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * model * vec4(vertex.xy, opacity,1.0);
    opacity = Opacity;
}
