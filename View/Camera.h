#pragma once
#include "../Math/MathLib.h"



typedef struct 
{
	Vec4 position;
	ViewDirection view;
	radian x_fov, y_fov;
} Camera;


void zoomCamera(Camera* cam, radian x_fov, radian y_fov);

radian evaluateFov(float distance);
