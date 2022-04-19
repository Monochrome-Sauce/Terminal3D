#pragma once
#include "SpaceObjects.h"
#include "Camera.h"
#include <stdio.h>


void renderPoint(_pIn_ const Camera* cam, _pIn_ const Point3D* point);


void clearGrid(void);
void printGrid(void);


const char* Point3D_toStr(_pIn_ const Point3D* x);

const char* Angle3D_toStr(_pIn_ const Angle3D* x);
