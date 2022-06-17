#include <stdio.h>
#include <memory.h>
#include "MatrixMath.h"



const char* Mat4x4_ToStr(const Mat4x4 m)
{
	static char s[256] = { 0 };
	
	int stride = 0;
	for (int i = 0; i < VEC_LEN; ++i)
	{
		const float* row = m[i];
		stride += sprintf(s + stride, "|%+.3f; %+.3f; %+.3f; %+.3f|\n",
								row[0], row[1], row[2], row[3]);
	}
	s[stride-1] = 0;
	return s;
}



Vec4* vec_cross_mat(Vec4* v, const Mat4x4 m)
{
	Vec4 u = { 0 };
	
	for (int row = 0; row < 4; ++row)
	{
		double sum = 0;
		
		for (int col = 0; col < 4; ++col)
		{
			sum += v->tuple[col] * m[row][col];
		}
		u.tuple[row] = sum;
	}
	
	*v = u;
	return v;
}


void vec_scale_mat(float s, Mat4x4 m)
{
	Mat4x4 tmp = {
		{s,0,0,0},
		{0,s,0,0},
		{0,0,s,0},
		{0,0,0,1},
	};
	memcpy(m, tmp, sizeof(Mat4x4));
}


void vec_xRotate_mat(float theta, Mat4x4 m)
{
	Mat4x4 tmp = {
		{1,0,0,0},
		{0,cos(theta),-sin(theta),0},
		{0,sin(theta),cos(theta),0},
		{0,0,0,1},
	};
	memcpy(m, tmp, sizeof(Mat4x4));
}

void vec_yRotate_mat(float theta, Mat4x4 m)
{
	Mat4x4 tmp = {
		{cos(theta),0,sin(theta),0},
		{0,1,0,0},
		{-sin(theta),0,cos(theta),0},
		{0,0,0,1},
	};
	memcpy(m, tmp, sizeof(Mat4x4));
}

void vec_zRotate_mat(float theta, Mat4x4 m)
{
	Mat4x4 tmp = {
		{cos(theta),-sin(theta),0,0},
		{sin(theta),cos(theta),0,0},
		{0,0,1,0},
		{0,0,0,1},
	};
	memcpy(m, tmp, sizeof(Mat4x4));
}
