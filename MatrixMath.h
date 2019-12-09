#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#define mat4(x) float x[16];
#define mat4i(x) float x[16];Mat4Identity(x);

typedef struct
{
	float x,y,z,w;
}vec4;

typedef struct
{
	float x,y,z;
}vec3;

void Mat4Identity(float *MatIn);
void Mat4TranslateSelf(float *InOut,vec3 *v);

void Mat4Ortho(float *Out,float left,float right,float bottom,
float top,float near,float far);

void Mat4Mult(float *a,float *b,float *Out);

void Mat4ScaleSelf(float *InOut,vec3 *v);

void Mat4Persp(float *m,float fov,
float aspect,float zNear,float zFar);

void Mat4SetTranslate(float *Mat,vec3 *v);

void Mat4LookAt(float *Out,vec3 *Eye,vec3 *Center,vec3 *Up);

void Mat4Zero(float *Out);

#endif
