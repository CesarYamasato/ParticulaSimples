#version 460 core

uniform sampler2D Textura;
out vec4 color;
in vec2 pos;
in float opacity;
void main(){
    vec3 col = texture(Textura,pos).xyz;
    vec3 temp = vec3(1.0);
    color = vec4(col.xyz*opacity,opacity);
}
