#pragma once

#include "../CodeTools.h"



typedef struct
{
	int16_t x, y, z;
} Point3D;


typedef struct
{
	double xz, yz;
} Angle3D;



typedef struct
{
	Point3D point[3];
} Polygon;



double calcDistance(_pIn_ const Point3D* p1, _pIn_ const Point3D* p2);
double calcAngle2D(int d1, int d2);
Angle3D calcAngle3D(_pIn_ const Point3D* p1, _pIn_ const Point3D* p2);

void renderPolygon(_pIn_ const Polygon* polygon);
