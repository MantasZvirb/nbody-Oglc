#include "Physics.h"
#include <math.h>
#include <stdio.h>

Particle *CreateParticleBuffer(unsigned long NumOfParticles)
{
	Particle *Pt = malloc(sizeof(Particle)*NumOfParticles);
	return Pt;
}

void SetParticle(Particle *Pd,
Particle *Buffer,unsigned long Index)
{
	Particle *Pp = &Buffer[Index];
	*Pp = *Pd;	
}

void RunSimulationFrameCpu(Particle *Pd,
unsigned long NumOfParticles,float dt)
{
	for(unsigned long i = 0; i < NumOfParticles;i++)
	{
		float Fx = 0;
		float Fy = 0;
		float Fz = 0;
		for(unsigned long i2 = 0;i2 < NumOfParticles;i2++)
		{
			if(i != i2)
			{
				float Distx = Pd[i].s.x - Pd[i2].s.x;
				float Disty = Pd[i].s.y - Pd[i2].s.y;
				float Distz = Pd[i].s.z - Pd[i2].s.z;
				
				float DistPow2 = pow(Distx,2)+pow(Disty,2)+pow(Distz,2);
				float Dist = sqrt(DistPow2);
				float DistPow6 = pow(DistPow2,4);
				
				float Force = (Pd[i].q*Pd[i2].q)/DistPow2;
				
				Fx+=(Distx/Dist)*Force;
				Fy+=(Disty/Dist)*Force;
				Fz+=(Distz/Dist)*Force;
			}
		}
		float vX = (Fx/Pd[i].m)*dt;
		float vY = (Fy/Pd[i].m)*dt;
		float vZ = (Fz/Pd[i].m)*dt;
		
		Pd[i].v.x += vX;
		Pd[i].v.y += vY;
		Pd[i].v.z += vZ;
		
		Pd[i].s.x += Pd[i].v.x*dt;
		Pd[i].s.y += Pd[i].v.y*dt;
		Pd[i].s.z += Pd[i].v.z*dt;
	}
}

void RunSimulationStepCpu(Particle *Pd,
unsigned long Pn,float dt,float Time)
{
	float Slice = Time/dt;
	for(float i = 0;i < Time;i+=Slice)
	{
		RunSimulationFrameCpu(Pd,Pn,dt);
	}
}



