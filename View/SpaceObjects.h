#pragma once

#include "../CodeTools.h"
#include <math.h>



typedef double radianAngle;
typedef double degreeAngle;

#define deg2rad(d) ((degreeAngle)(((d) * M_PI) / 180))
#define rad2deg(r) ((radianAngle)(((r) * 180) / M_PI))

#define roundRadians(r) (radianAngle)(remainder(r, 2*M_PI))
#define roundDegrees(d) (degreeAngle)(remainder(d, 360))



typedef struct
{
	float x, y, z;
} Point3D;

Point3D* mulPoint3D(_pOut_ Point3D* point, int scalar);
Point3D* addPoint3D(_pOut_ Point3D* dst, _pIn_ const Point3D* src);



typedef struct
{
	radianAngle xz, yz;
} Angle3D;

Angle3D* addAngle3D(_pOut_ Angle3D* dst, _pIn_ const Angle3D* src);
Angle3D* subAngle3D(_pOut_ Angle3D* dst, _pIn_ const Angle3D* src);

double calcDistance(_pIn_ const Point3D* p1, _pIn_ const Point3D* p2);



typedef struct
{
	Point3D point[3];
} Polygon;
