#include <GL/gl.h>
#include <GL/glu.h>
#include "Physics.h"
#include "Utils.h"
#include "MatrixMath.h"

GLuint VertexShaderId = 0;
GLuint FragmentShaderId = 0;
GLuint LinkedShaderId = 0;

GLuint ComputeShaderId = 0;
GLuint LinkedComputeId = 0;

GLuint SphereVertexBuff = 0;
GLuint SphereIndexBuff = 0;

GLuint ComBuff = 0;

GLuint NumOfTris;
GLuint NumOfVerts;

GLint UniformIdm1;
GLint UniformIdDt;
GLint UniformIdm2;
GLint UniformIdsr;

GLuint ParticleNum; 

GLuint VAO;


char GlCheckError(void)
{
	GLuint Errors[] = 
	{GL_NO_ERROR,
	GL_INVALID_ENUM,
	GL_INVALID_OPERATION,
	GL_INVALID_OPERATION,
	GL_INVALID_ENUM,
	GL_OUT_OF_MEMORY,
	GL_INVALID_OPERATION};

	GLuint err = glGetError();
	for(int i = 0;i < sizeof(Errors)/sizeof(GLuint);i++)
	{
		if(Errors[i] == err)
			return i;
	}
	return err;
}


char SetParticleState(Particle *DtaPtr,
unsigned long PartNum,char Mode)
{
	GLuint Modes[] = 
	{
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW
	};
	
	if(Mode > 2)
	{
		return 0;
	}
	
	GLuint nSz = PartNum*sizeof(Particle);
	ParticleNum = PartNum;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ComBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER,nSz,DtaPtr,Modes[Mode]);
	return 1;
}

void RunSimulationFrame(float dt)
{
	float dta = dt;
	glUseProgram(LinkedComputeId);
	glUniform1fv(UniformIdDt,1,&dta);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,ComBuff);
	glDispatchCompute(ParticleNum,1,1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GetParticleState(Particle *OutPtr,unsigned long PartNum)
{	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ComBuff);
	Particle *Pb = glMapBuffer(GL_SHADER_STORAGE_BUFFER,GL_READ_ONLY);
	for(unsigned long i = 0; i < PartNum;i++)
	{
		OutPtr[i] = Pb[i]; 
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void RunSimulationStep(float dt,float Time)
{
	float dta = dt;
	glUseProgram(LinkedComputeId);
	glUniform1fv(UniformIdDt,1,&dta);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,ComBuff);
	for(float i = 0;i < Time;i+=dt)
	{
		glDispatchCompute(ParticleNum,1,1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}

void SetSystemVolume(float sr)
{
	float dta = sr;
	glUseProgram(LinkedComputeId);
	glUniform1fv(UniformIdsr,1,&dta);
}


char CompileShader(char *ShaderSource,unsigned long *SourceLen,
GLuint *OutId,GLuint Type)
{
	*OutId = glCreateShader(Type);
	glShaderSource(*OutId,1,&ShaderSource,SourceLen);
	glCompileShader(*OutId);
	
	GLint isCompiled = 0;
	glGetShaderiv(*OutId,GL_COMPILE_STATUS,&isCompiled);
	if(isCompiled == GL_FALSE)
	{
		char Log[1024*10];
		unsigned long LogLenght = 1024*10;
		glGetShaderInfoLog(*OutId,LogLenght,0,&Log);
		pc(ShaderSource);
		pc("Shader Compile:");		
		pc(Log);
		glDeleteShader(*OutId);
		return 0;
	}
	return 1;
}

char LinkShaders(GLuint VertShader,GLuint FragShader,
GLuint *OutLinkedProgram)
{
	*OutLinkedProgram = glCreateProgram();
	if(VertShader != 0)
	{
		glAttachShader(*OutLinkedProgram,VertShader);
	}
	glAttachShader(*OutLinkedProgram,FragShader);
	glLinkProgram(*OutLinkedProgram);

	GLint isLinked = 0;
	glGetProgramiv(*OutLinkedProgram,GL_LINK_STATUS,&isLinked);
	if (isLinked == GL_FALSE)
	{
		char Log[1024*10];
		unsigned long LogSize = 1024*10;
		glGetProgramInfoLog(*OutLinkedProgram,LogSize,&LogSize,&Log);
		pc("Shader Link Error:");
		pc(Log);
		glDeleteProgram(*OutLinkedProgram);
		return 0;
	}
	return 1; 
}

mat4(Vm);
mat4(Prp);

vec3 Eye = {0};
vec3 Center = {0};
vec3 Up = {0};

void DrawParticles(void)
{
	//Mat4SetTranslate(Vm,&Eye);
	Mat4LookAt(Vm,&Eye,&Center,&Up);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(LinkedShaderId);
	glUniformMatrix4fv(UniformIdm2,1,GL_FALSE,Vm);
	glBindVertexArray(VAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,ComBuff);
	glDrawElementsInstanced(GL_TRIANGLES,NumOfVerts,
	GL_UNSIGNED_SHORT,0,ParticleNum);
}

void DrawFrame(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(LinkedShaderId);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,NumOfVerts,GL_UNSIGNED_SHORT,0);
}



void TranslateCamera(float x,float y,float z)
{
	Eye.x += x;
	Eye.y += y;
	Eye.z += z;
	Center.x += x;
	Center.y += y;
}

char InitRender(void)
{
	unsigned long Vts = 0;
	unsigned long Fts = 0;
	unsigned long Cts = 0;
	char *VertexShaderText = LoadFile("./Assets/VertexShader.txt",&Vts);
	char *FragmentShaderText = LoadFile("./Assets/FragmentShader.txt",&Fts);
	char *ComputeShaderText = LoadFile("./Assets/ComputeShader.txt",&Cts);
	if(VertexShaderText == 0)
	{
		pc("Cant Load Vertex Shader");
		return 0;
	}
	if(FragmentShaderText == 0)
	{
		pc("Cant Load Fragment Shader");
		return 0;
	}
	if(CompileShader(VertexShaderText,&Vts,
	&VertexShaderId,GL_VERTEX_SHADER) != 1)
	{
		return 0;
	}
	if(CompileShader(FragmentShaderText,&Fts,
	&FragmentShaderId,GL_FRAGMENT_SHADER) != 1)
	{
		return 0;
	}
	if(CompileShader(ComputeShaderText,&Cts,
	&ComputeShaderId,GL_COMPUTE_SHADER) != 1)
	{
		return 0;
	}
	
	if(LinkShaders(VertexShaderId,
	FragmentShaderId,&LinkedShaderId) != 1)
	{
		return 0;
	}
	
	if(LinkShaders(0,ComputeShaderId,&LinkedComputeId) != 1)
	{
		return 0;
	}
	glClearColor(0,0,0.3,1);
	
	UniformIdm1 = glGetUniformLocation(LinkedShaderId,"m1");
	UniformIdDt = glGetUniformLocation(LinkedComputeId,"dt");
	UniformIdsr = glGetUniformLocation(LinkedComputeId,"sr");
	UniformIdm2 = glGetUniformLocation(LinkedShaderId,"m2");
	
	glUseProgram(LinkedShaderId);
	
	Mat4Persp(Prp,45.0,4.0/3.0,0.1,1000.0);
	glUniformMatrix4fv(UniformIdm1,1,GL_FALSE,Prp);
	Mat4Identity(Vm);
	
	Up.y = 1;
	Center.z = -1;
		
	//glPolygonMode( GL_FRONT_AND_BACK,GL_LINE);
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	
	unsigned long Sds;
	char *SphereData = LoadFile("./Assets/SphereData.txt",&Sds);
	
	float *VertexDta;
	unsigned long VertexDtaSize;
	
	ChunkToData(SphereData,Sds,"Vertex",
	&VertexDtaSize,&VertexDta,0);
	
	float *NormalDta;
	unsigned long NormalDtaSize;
	
	ChunkToData(SphereData,Sds,"Normal",
	&NormalDtaSize,&NormalDta,0);
	
	unsigned short *IndexDta;
	unsigned long IndexDtaSize;
	
	ChunkToData(SphereData,Sds,"Index",
	&IndexDtaSize,0,&IndexDta);
	
	NumOfTris = (IndexDtaSize/sizeof(short))/3;
	NumOfVerts = (IndexDtaSize/sizeof(short));
	
	float *VertNormBuff = malloc(VertexDtaSize*2);
	
	unsigned long BuffIndx = 0;
	for(unsigned long i = 0; i < (VertexDtaSize/sizeof(float));i+=3)
	{
		VertNormBuff[BuffIndx] = VertexDta[i];
		BuffIndx++;
		VertNormBuff[BuffIndx] = VertexDta[i+1];
		BuffIndx++;
		VertNormBuff[BuffIndx] = VertexDta[i+2];
		BuffIndx++;
		VertNormBuff[BuffIndx] = NormalDta[i];
		BuffIndx++;
		VertNormBuff[BuffIndx] = NormalDta[i+1];
		BuffIndx++;
		VertNormBuff[BuffIndx] = NormalDta[i+2];
		BuffIndx++;
	}
	
	GLuint BuffArray[3];
	glGenBuffers(3,BuffArray);
	
	SphereVertexBuff = BuffArray[0];
	SphereIndexBuff = BuffArray[1];
	
	glBindBuffer(GL_ARRAY_BUFFER,SphereVertexBuff);
	
	glBufferData(GL_ARRAY_BUFFER,VertexDtaSize*2,
	VertNormBuff,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,SphereIndexBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,IndexDtaSize,
	IndexDta,GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
	SphereIndexBuff);
	
	glBindBuffer(GL_ARRAY_BUFFER,SphereVertexBuff);
	
	//Vertex
	glVertexAttribPointer(0,3,GL_FLOAT,
	GL_FALSE,sizeof(float)*6,0);
	
	//Normal
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,
	sizeof(float)*6,sizeof(float)*3);
	
	ComBuff = BuffArray[2];
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ComBuff);
	
	return 1;
}
