#pragma once
#include <math.h>



#define VEC_LEN 4
typedef union
{
	float tuple[VEC_LEN];
	struct {
		float x, y, z, w;
	};
} Vec4;

const char* Vec4_ToStr(const Vec4* v);



Vec4* vec_add3_vec(Vec4* v, const Vec4* u);
Vec4* vec_sub3_vec(Vec4* v, const Vec4* u);

Vec4* vec_add4_vec(Vec4* v, const Vec4* u);
Vec4* vec_sub4_vec(Vec4* v, const Vec4* u);

Vec4* vec_dot_scallar(Vec4* v, float s);


Vec4* vec_project(Vec4* v);
