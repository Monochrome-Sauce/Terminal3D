#include "Camera.h"



void rotateCameraHorizontally(_pIn_ Camera* cam, radianAngle angle)
{
	cam->direction.xz = roundRadians(cam->direction.xz + angle);
}


void rotateCameraVertically(_pIn_ Camera* cam, radianAngle angle)
{
	cam->direction.yz = roundRadians(cam->direction.yz + angle);
}
