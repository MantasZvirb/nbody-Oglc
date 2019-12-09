#include <math.h>
#include "MatrixMath.h"

void Mat4Identity(float *MatIn)
{
	MatIn[0] = 1.0;
    MatIn[1] = 0.0;
    MatIn[2] = 0.0;
    MatIn[3] = 0.0;
    MatIn[4] = 0.0;
    MatIn[5] = 1.0;
   	MatIn[6] = 0.0;
    MatIn[7] = 0.0;
    MatIn[8] = 0.0;
    MatIn[9] = 0.0;
    MatIn[10] = 1.0;
    MatIn[11] = 0.0;
    MatIn[12] = 0.0;
    MatIn[13] = 0.0;
    MatIn[14] = 0.0;
    MatIn[15] = 1.0;
}

void Mat4TranslateSelf(float *InOut,vec3 *v)
{
	 InOut[12] = InOut[0] * v->x + InOut[4] * v->y + InOut[8] * v->z + InOut[12];
     InOut[13] = InOut[1] * v->x + InOut[5] * v->y + InOut[9] * v->z + InOut[13];
     InOut[14] = InOut[2] * v->x + InOut[6] * v->y + InOut[10] * v->z + InOut[14];
     InOut[15] = InOut[3] * v->x + InOut[7] * v->y + InOut[11] * v->z + InOut[15];
}

void Mat4SetTranslate(float *Mat,vec3 *v)
{
	Mat[3] = v->x;
	Mat[7] = v->y;
	Mat[11] = v->z;
}

void Mat4ScaleSelf(float *out,vec3 *v)
{
	out[0]*=v->x;
    out[1]*=v->x;
    out[2]*=v->x;
    out[3]*=v->x;
    out[4]*=v->y;
    out[5]*=v->y;
    out[6]*=v->y;
    out[7]*=v->y;
    out[8]*=v->z;
    out[9]*=v->z;
    out[10]*=v->z;
    out[11]*=v->z;
}

void Mat4Mult(float *a,float *b,float *Out)
{
	float a00 = a[0], a01 = a[1], a02 = a[2], a03 = a[3],
	a10 = a[4], a11 = a[5], a12 = a[6], a13 = a[7],
	a20 = a[8], a21 = a[9], a22 = a[10], a23 = a[11],
	a30 = a[12], a31 = a[13], a32 = a[14], a33 = a[15];
	
	float b0  = b[0], b1 = b[1], b2 = b[2], b3 = b[3];  
	
	Out[0] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    Out[1] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    Out[2] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    Out[3] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

	b0 = b[4]; b1 = b[5]; b2 = b[6]; b3 = b[7];
    Out[4] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    Out[5] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    Out[6] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    Out[7] = b0*a03 + b1*a13 + b2*a23 + b3*a33;
    
    b0 = b[8]; b1 = b[9]; b2 = b[10]; b3 = b[11];
    Out[8] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    Out[9] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    Out[10] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    Out[11] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

    b0 = b[12]; b1 = b[13]; b2 = b[14]; b3 = b[15];
    Out[12] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    Out[13] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    Out[14] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    Out[15] = b0*a03 + b1*a13 + b2*a23 + b3*a33;
}


void Mat4Ortho(float *Out,float left,float right,float bottom,
float top,float near,float far)
{
	float lr = 1/(left-right);
	float bt = 1/(bottom-top);
	float nf = 1/(near-far);
	
	Out[0] = -2.0*lr;
	Out[1] = 0;
	Out[2] = 0;
	Out[3] = 0;
	Out[4] = 0;
	Out[5] = -2.0*bt;
	Out[6] = 0;
	Out[7] = 0;
	Out[8] = 0;
	Out[9] = 0;
	Out[10] = 2.0*nf;
	Out[11] = 0;
	Out[12] = (left+right)*lr;
	Out[13] = (top+bottom)*bt;
	Out[14] = (far+near)*nf;
	Out[15] = 1.0;
}



void Mat4Fustrum(float *Out,float l,float r,
float b,float t,float n,float f)
{
	float rl = (r-l);
	float tb = (t-b);
	float fn = (f-n);
	
	Out[0] = (n*2.0)/rl;
    Out[1] = 0;
    Out[2] = 0;
    Out[3] = 0;
    Out[4] = 0;
    Out[5] = (n*2.0)/tb;
    Out[6] = 0;
    Out[7] = 0;
    Out[8] = (r+l)/rl;
    Out[9] = (t+b)/tb;
    Out[10] = -(f+n)/fn;
    Out[11] = -1.0;
    Out[12] = 0;
    Out[13] = 0;
    Out[14] = -(f*n*2.0)/fn;
	Out[15] = 0;
}

void Mat4Zero(float *Out)
{
	for(unsigned char i = 0; i < 16;i++)
	{
		Out[i] = 0.0;
	}
}

void mat4xy(float *m,char x,char y,float d)
{
	m[y*4+x] = d;
}


void Mat4Persp(float *m,float fov,
float aspect,float zNear,float zFar)
{
	const float h = 1.0f/tan(fov*(3.14/360.0));
	float neg_depth = zNear-zFar;
	m[0] = h/aspect;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;
	m[4] = 0;
	m[5] = h;
	m[6] = 0;
	m[7] = 0;
	m[8] = 0;
	m[9] = 0;
	m[10] = (zFar + zNear)/neg_depth;
	m[11] = -1;
	m[12] = 0;
	m[13] = 0;
	m[14] = 2.0f*(zNear*zFar)/neg_depth;
	m[15] = 0;
}

void Mat4LookAt(float *Out,vec3 *Eye,vec3 *Center,vec3 *Up)
{
	float x0, x1, x2, y0, y1, y2, z0, z1, z2, len,
	eyex = Eye->x,
	eyey = Eye->y,
	eyez = Eye->z,
	upx = Up->x,
	upy = Up->y,
	upz = Up->z,
	centerx = Center->x,
	centery = Center->y,
	centerz = Center->z;
     
	z0 = eyex - centerx;
	z1 = eyey - centery;
	z2 = eyez - centerz;
	
	len = 1.0 / sqrt(z0 * z0 + z1 * z1 + z2 * z2);
	z0 *= len;
	z1 *= len;
	z2 *= len;
	
	x0 = upy * z2 - upz * z1;
    x1 = upz * z0 - upx * z2;
    x2 = upx * z1 - upy * z0;
    len = sqrt(x0 * x0 + x1 * x1 + x2 * x2);
    if (!len) 
    {
        x0 = 0;
        x1 = 0;
        x2 = 0;
    } 
	else 
	{
		len = 1.0 / len;
		x0 *= len;
		x1 *= len;
		x2 *= len;
	}
	
	y0 = z1 * x2 - z2 * x1;
	y1 = z2 * x0 - z0 * x2;
	y2 = z0 * x1 - z1 * x0;
	
	
	len = sqrt(y0 * y0 + y1 * y1 + y2 * y2);
    if(!len) 
    {
    	y0 = 0;
		y1 = 0;
		y2 = 0;
    } 
    else 
    {
		len = 1.0 / len;
		y0 *= len;
		y1 *= len;
		y2 *= len;
	}
	Out[0] = x0;
    Out[1] = y0;
    Out[2] = z0;
    Out[3] = 0;
    Out[4] = x1;
    Out[5] = y1;
    Out[6] = z1;
    Out[7] = 0;
    Out[8] = x2;
    Out[9] = y2;
    Out[10] = z2;
    Out[11] = 0;
    Out[12] = -(x0 * eyex + x1 * eyey + x2 * eyez);
    Out[13] = -(y0 * eyex + y1 * eyey + y2 * eyez);
    Out[14] = -(z0 * eyex + z1 * eyey + z2 * eyez);
	Out[15] = 1.0;
}



