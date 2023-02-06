#version 460 core

uniform sampler2D bufferB;
uniform int Frame;
in vec4 mouse;
in vec2 R;
in float time;
in vec2 pos;

out vec4 U;

#include <common.fs>

void main( )
{
    ivec2 p = ivec2(pos);
    vec4 data = texel(bufferB, pos); 

    particle P;// = getParticle(data, pos);
    P.X = vec2(0.);
    P.V = vec2(0.);
    P.M = vec2(0.);
    
    Reintegration(bufferB, P, pos);
   
    //initial condition
    if(Frame < 1)
    {
        //random
        vec3 rand = hash32(pos); //hash32 = general kernel function
        if(rand.z < 0.0) 
        {
            P.X = pos;
            P.V = 0.5*(rand.xy-0.5) + vec2(0., 0.);
            P.M = vec2(mass, 0.);
        }
        else
        {
            P.X = pos;
            P.V = vec2(0.0);
            P.M = vec2(0.);
        }
    }
    
    
    U = saveParticle(P, pos);
}
