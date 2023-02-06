#version 460 core

uniform sampler2D bufferA;
in vec4 mouse;
in vec2 R;
in float time;
in vec2 pos;

out vec4 U;

#include <common.fs>

void main()
{
    ivec2 p = ivec2(pos);
        
    vec4 data = texel(bufferA, pos); 
    
    particle P = getParticle(data, pos);
    
    
    if(P.M.x != 0.) //not vacuum
    {
        Simulation(bufferA, P, pos);
    }
    
    if(length(P.X - R*vec2(0.8, 0.9)) < 20.)//se estiver dentro do alcance da área de criação das partículas
    {
        P.X = pos;
        P.V = 0.5*Dir(-PI*0.25 - PI*0.5 + 0.3*sin(0.4*time));
        P.M = mix(P.M, vec2(fluid_rho, 1.), 0.4);
    }

    if(length(P.X - R*vec2(0.2, 0.9)) < 20.)//se estiver dentro do alcance da área de criação das partículas
    {
        P.X = pos;
        P.V = 0.5*Dir(-PI*0.25 + 0.3*sin(0.3*time));
        P.M = mix(P.M, vec2(fluid_rho, 0.1), 0.4);
    }
    
    U = saveParticle(P, pos);
}
