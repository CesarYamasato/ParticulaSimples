#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TextCoord;
uniform vec4 Mouse;
uniform vec2 Resolution;
uniform float Time;
uniform int Frame;
out float time;
out vec2 R;
out vec4 mouse;
out int frame;
out vec2 pos;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    mouse = Mouse;
    R = Resolution;
    time = Time;
    pos = TextCoord;
}
