#version 460 core

uniform sampler2D bufferA;
in vec2 R;
in float time;
in vec4 mouse;
in vec2 pos;

out vec4 fragColor;

#include <common.fs>

void main()
{
    ivec2 p = ivec2(pos);

    vec4 data = texel(bufferA, pos);
    particle P = getParticle(data, pos);
    
    //particle render
    vec4 rho = vec4(0.);
    range(i, -1, 1) range(j, -1, 1)
    {
        vec2 ij = vec2(i,j);
        vec4 data = texel(bufferA, pos + ij);
        particle P0 = getParticle(data, pos + ij);

        vec2 x0 = P0.X; //update position
        //how much mass falls into this pixel
        rho += 1.*vec4(P.V, P.M)*G((pos - x0)/0.75); 
    }
    
    fragColor = rho;
}
