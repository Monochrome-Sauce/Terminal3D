#pragma once
#include "SpaceObjects.h"



typedef struct 
{
	Point3D position;
	Angle3D direction;
	Angle3D field_of_view;
} Camera;


void rotateCameraHorizontally(_pIn_ Camera* cam, radianAngle angle);
void rotateCameraVertically(_pIn_ Camera* cam, radianAngle angle);

radianAngle evaluateFov(float distance);
