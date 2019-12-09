#include "Physics.h"
#include "MatrixMath.h"

char InitRender(void);
void DrawFrame(void);

char SetParticleState(Particle *DtaPtr,
unsigned long PartNum,char Mode);

void GetParticleState(Particle *OutPtr,unsigned long PartNum);
void RunSimulationFrame(float dt);
void RunSimulationStep(float dt,float Time);
void SetSystemVolume(float sr);
void DrawParticles(void);

void TranslateCamera(float x,float y,float z);

void DrawFr(void);
