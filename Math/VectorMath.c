#include "VectorMath.h"
#include <stdio.h>



const char* Vec4_ToStr(const Vec4* v)
{
	static char s[128] = { 0 };
	sprintf(s, "Vec4(%+.3f; %+.3f; %+.3f; %+.3f)", v->x, v->y, v->z, v->w);
	return s;
}



Vec4* vec_add3_vec(Vec4* v, const Vec4* u)
{
	for (int i = 0; i < 3; ++i)
	{
		v->tuple[i] += u->tuple[i];
	}
	return v;
}


Vec4* vec_sub3_vec(Vec4* v, const Vec4* u)
{
	for (int i = 0; i < 3; ++i)
	{
		v->tuple[i] -= u->tuple[i];
	}
	return v;
}



Vec4* vec_add4_vec(Vec4* v, const Vec4* u)
{
	v->w += u->w;
	return vec_add3_vec(v, u);
}


Vec4* vec_sub4_vec(Vec4* v, const Vec4* u)
{
	v->w -= u->w;
	return vec_sub3_vec(v, u);
}



Vec4* vec_dot_scallar(Vec4* v, float s)
{
	for (int i = 0; i < VEC_LEN; ++i)
	{
		v->tuple[i] *= s;
	}
	return v;
}



Vec4* vec_project(Vec4* v)
{
	v->w *= v->z;
	v->z *= v->z;
	return vec_dot_scallar(v, 1/(v->w));
}
