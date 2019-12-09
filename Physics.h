#ifndef PHYSICS_H
#define PHYSICS_H
#include "MatrixMath.h"

typedef struct
{
	vec4 s;//Location
	vec4 v;//Speed
	float q;//Charge
	float m;//Mass
	float r;//radius
	float rf;//repulsion force
}Particle;

Particle *CreateParticleBuffer(unsigned long NumOfParticles);
void SetParticle(Particle *Pd,Particle *Buffer,unsigned long Index);


void RunSimulationFrameCpu(Particle *Pd,
unsigned long NumOfParticles,float dt);

void RunSimulationStepCpu(Particle *Pd,
unsigned long Pn,float dt,float Time);

#endif
