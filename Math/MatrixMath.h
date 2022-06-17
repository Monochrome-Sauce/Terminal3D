#pragma once
#include "VectorMath.h"


typedef float Mat4x4[VEC_LEN][VEC_LEN];

const char* Mat4x4_ToStr(const Mat4x4 m);



Vec4* vec_cross_mat(Vec4* v, const Mat4x4 m);


void vec_xRotate_mat(float theta, Mat4x4 m);
void vec_yRotate_mat(float theta, Mat4x4 m);
void vec_zRotate_mat(float theta, Mat4x4 m);
