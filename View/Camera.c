#include "Camera.h"
#include <stdio.h>
#include <unistd.h>



static const radian MIN_FOV = deg2rad(10);
static const radian MAX_FOV = deg2rad(160);


static float limitedFov(radian fov)
{
	if (fov < MIN_FOV)
	{
		return MIN_FOV;
	}
	else if (fov > MAX_FOV)
	{
		return MAX_FOV;
	}
	
	return fov;
}


void zoomCamera(Camera* cam, radian x_fov, radian y_fov)
{
	cam->x_fov = limitedFov(cam->x_fov - x_fov);
	cam->y_fov = limitedFov(cam->y_fov - y_fov);
}


radian evaluateFov(float distance)
{
	return atan(1/distance) * 2;
}
