#include "ViewPortMath.h"



bool isValidPos(float x, float y)
{
	return IN_RANGE(-1.0, x, +1.0) && IN_RANGE(-1.0, y, +1.0);
}

float calcRelPosIn2D(radian fov, float depth, float posInSpace)
{
	return posInSpace/(tan(fov/2) * depth);
}

int widthIndex(float relx)
{
	return floor((relx+1)*(GRID_W/2.0));
}

int heightIndex(float rely)
{
	return floor((rely+1)*(GRID_H/2.0));
}




//using a Bezier Curve equation for 2 points:
//	P = (1-t)*p1 + (t)*p2
//	x = (1-t)*x1 + (t)*x2
//	x = x1-t*x1 + t*x2 = x1 + t*(-x1 + x2)
//	t = (x-x1)/(x2-x1)
static float bezierCurveIntersectionDistance(float intersection, float a, float b)
{
	return (intersection - a)/(b - a);
}

static float bezierCurveIntersectionPos(float t, float a, float b)
{
	return (1-t)*a + t*b;
}



bool findRightIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2)
{
	res->x = +1;	//lim(x)
	
	float t = bezierCurveIntersectionDistance(res->x, p1->x, p2->x);
	if (t < 0 || t > 1)
		return false;
	
	res->y = bezierCurveIntersectionPos(t, p1->y, p2->y);
	return true;
}

bool findLeftIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2)
{
	res->x = -1;	//lim(x)
	
	float t = bezierCurveIntersectionDistance(res->x, p1->x, p2->x);
	if (t < 0 || t > 1)
		return false;
	
	res->y = bezierCurveIntersectionPos(t, p1->y, p2->y);
	return true;
}

bool findTopIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2)
{
	res->y = +1;	//lim(y)
	
	float t = bezierCurveIntersectionDistance(res->y, p1->y, p2->y);
	if (t < 0 || t > 1)
		return false;
	
	res->x = bezierCurveIntersectionPos(t, p1->x, p2->x);
	return true;
}

bool findBottomIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2)
{
	res->y = -1;	//lim(y)
	
	float t = bezierCurveIntersectionDistance(res->y, p1->y, p2->y);
	if (t < 0 || t > 1)
		return false;
	
	res->x = bezierCurveIntersectionPos(t, p1->x, p2->x);
	return true;
}
