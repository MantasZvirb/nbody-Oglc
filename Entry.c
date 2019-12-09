#include <stdio.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "Render.h"
#include "Physics.h"


float frand(float min,float max )
{
    float scale = rand()/(float)RAND_MAX;//[0, 1.0]
    return min+scale*(max-min);//[min, max]
}

void FrameRender(SDL_Window *Window)
{
	DrawParticles();
	SDL_GL_SwapWindow(Window);
}

int main(int argc,char *argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return 0;
	}
	
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
     
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
	SDL_GL_CONTEXT_PROFILE_CORE);
	

	SDL_Window *Window = SDL_CreateWindow(
	"Particles",0,0,640,480,SDL_WINDOW_OPENGL);
	
	SDL_GLContext GlContext = SDL_GL_CreateContext(Window);
	if(GlContext == 0)
	{
		pc("Opengl Context");
		return 0;
	}
	
	if(InitRender() == 0)
	{
		pc("Cant Init Render");
		return 0;
	}
	
	
	Particle Pp = {0};
	Pp.r = 0.2;
	Pp.q = 1;
	Pp.m = 1;
	Pp.s.z = -10;
	unsigned long Pn = 100;
	Particle *Pb = CreateParticleBuffer(Pn);
	Pb[0] = Pp;

	SetSystemVolume(1000);
	for(unsigned long i = 1;i < Pn;i++)
	{
		Pp.s.x = frand(-10,10);
		Pp.s.y = frand(-10,10);
		Pp.s.z = frand(-10,10);
		Pp.q = frand(-5,1);
		if(Pp.q < 0)
		{
			Pp.m = 0.0001;
			Pp.r = 0.2;
			Pp.q = -1;
		}
		else
		{
			Pp.m = 1;
			Pp.r = 0.4;
			Pp.q = 1;
		}


		SetParticle(&Pp,Pb,i);
	}
	SetParticleState(Pb,Pn,0);
	
	SDL_Event Evnt;
	unsigned char Quit = 0;
	while(Quit != 1)
	{
		FrameRender(Window);
		RunSimulationStep(0.0001,0.01);
		
		while(SDL_PollEvent(&Evnt) != 0)
		{
			if(Evnt.type == SDL_QUIT)
			{
				Quit = 1;
				break;
			}
			if(Evnt.type == SDL_KEYDOWN)
			{
				if(Evnt.key.keysym.sym == SDLK_UP)
				{
					TranslateCamera(0,0,-0.5);
				}
				if(Evnt.key.keysym.sym == SDLK_DOWN)
				{
					TranslateCamera(0,0,0.5);
				}
				if(Evnt.key.keysym.sym == SDLK_RIGHT)
				{
					TranslateCamera(0.5,0,0);
				}
				if(Evnt.key.keysym.sym == SDLK_LEFT)
				{
					TranslateCamera(-0.5,0,0);
				}
				if(Evnt.key.keysym.sym == SDLK_z)
				{
					TranslateCamera(0,-0.5,0);
				}
				if(Evnt.key.keysym.sym == SDLK_x)
				{
					TranslateCamera(0,0.5,0);
				}
			}
		}
	}
	SDL_GL_DeleteContext(GlContext);
	return 0;
}
