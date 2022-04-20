#include "SpaceObjects.h"



Angle3D* addAngle3D(_pOut_ Angle3D* dst, _pIn_ const Angle3D* src)
{
	dst->xz = roundRadians(dst->xz + src->xz);
	dst->yz = roundRadians(dst->yz + src->yz);
	return dst;
}


Angle3D* subAngle3D(_pOut_ Angle3D* dst, _pIn_ const Angle3D* src)
{
	dst->xz = roundRadians(dst->xz - src->xz);
	dst->yz = roundRadians(dst->yz - src->yz);
	return dst;
}



Point3D* mulPoint3D(_pOut_ Point3D* point, int scalar)
{
	point->x *= scalar;
	point->y *= scalar;
	point->z *= scalar;
	return point;
}


Point3D* addPoint3D(_pOut_ Point3D* dst, _pIn_ const Point3D* src)
{
	dst->x += src->x;
	dst->y += src->y;
	dst->z += src->z;
	return dst;
}


double calcDistance(_pIn_ const Point3D* p1, _pIn_ const Point3D* p2)
{
	int xd = p2->x - p1->x;
	int yd = p2->y - p1->y;
	int zd = p2->z - p1->z;
	return sqrt(xd*xd + yd*yd + zd*zd);
}
