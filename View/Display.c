#include "Display.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define GOTO_XY(x,y) printf("\033[%d;%dH", (y), (x)); fflush(stdout)

#define IN_RANGE(low, val, high) ((low)<=(val) && (val)<=(high))

#define DEG_TO_RAD(degrees) (((degrees) * M_PI) / 180)
#define RAD_TO_DEG(radians) (((radians) * 180) / M_PI)



double roundDegrees(double theta)
{
	return fmod(360 + theta, 360);
}


double roundTowardsZero(double x)
{
	return (x > 0) ? floor(x) : ceil(x);
}



const char* vec3str(const Point3D* v)
{
	static char buffer[64] = { 0 };
	sprintf(buffer, "{%d,%d,%d}", v->x, v->y, v->z);
	return buffer;
}



double calcDistance(const Point3D* p1, const Point3D* p2)
{
	int xd = p2->x - p1->x;
	int yd = p2->y - p1->y;
	int zd = p2->z - p1->z;
	return sqrt(xd*xd + yd*yd + zd*zd);
}




double calcAngle2D(int d1, int d2)
{
	double theta = 0;
	
	if (d2 != 0)
	{
		theta = atan(d1 / (double)d2);
	}
	else if (d1 != 0)
	{
		theta = asin(d1 / abs(d1));
	}
	
	theta = RAD_TO_DEG(theta) + 180*(d2 < 0);
	return roundDegrees(theta);
}



Angle3D calcAngle3D(const Point3D* p1, const Point3D* p2)
{
	int xd = p2->x - p1->x;
	int yd = p2->y - p1->y;
	int zd = p2->z - p1->z;
	
	Angle3D a = {
		.xz = calcAngle2D(xd, zd),
		.yz = calcAngle2D(yd, zd),
	};
	return a;
}



void renderPolygon(const Polygon* polygon)
{
	GOTO_XY(0, 0);
	Point3D viewer = { .x = 0, .y = 0, .z = 0 };
	
	Angle3D theta = calcAngle3D(&viewer, &polygon->point[0]);
	
	printf("%s | h = %.3f | v = %.3f          \n", vec3str(polygon->point), theta.xz, theta.yz);
}

