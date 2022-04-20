#include "Display.h"
#include "SpaceObjects.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define GOTO_XY(x,y) printf("\033[%d;%dH", (y), (x)); fflush(stdout)

#define IN_RANGE(low, val, high) ((low)<=(val) && (val)<=(high))



static Angle3D calcAngle3D(_pIn_ const Point3D* p1, _pIn_ const Point3D* p2)
{
	int xd = p2->x - p1->x;
	int yd = p2->y - p1->y;
	int zd = p2->z - p1->z;
	
	Angle3D a = {
		.xz = atan2(xd, zd),
		.yz = atan2(yd, zd),
	};
	return a;
}



const char* Point3D_toStr(_pIn_ const Point3D* x)
{
	static char buffer[64] = { 0 };
	sprintf(buffer, "{%.3f,%.3f,%.3f}", x->x, x->y, x->z);
	return buffer;
}

const char* Angle3D_toStr(_pIn_ const Angle3D* x)
{
	static char buffer[64] = { 0 };
	sprintf(buffer, "{%.3f,%.3f}", rad2deg(x->xz), rad2deg(x->yz));
	return buffer;
}



#define GRID_H 60
#define GRID_W 120

char grid[GRID_H][GRID_W] = { 0 };



void renderPoint(_pIn_ const Camera* cam, _pIn_ const Point3D* point)
{
	Angle3D theta = calcAngle3D(&cam->position, point);
	subAngle3D(&theta, &cam->direction);
	
	if (!(IN_RANGE(-cam->field_of_view.xz/2, theta.xz, cam->field_of_view.xz/2) &&
		  IN_RANGE(-cam->field_of_view.yz/2, theta.yz, cam->field_of_view.yz/2)))
	{
		return;
	}
	
	
	double point_distance = calcDistance(&cam->position, point);
	
	double y_fov_offset = point_distance * cos(theta.yz) * tan(cam->field_of_view.yz/2);
	double y_point_offset = point_distance * sin(theta.yz);
	double y = y_point_offset / y_fov_offset;
	
	double x_fov_offset = point_distance * cos(theta.xz) * tan(cam->field_of_view.xz/2);
	double x_point_offset = point_distance * sin(theta.xz);
	double x = x_point_offset / x_fov_offset;
	
	if (IN_RANGE(-1, x, 1) && IN_RANGE(-1, y, 1))
	{
		int index_y = floor((y+1) * GRID_H/2);
		int index_x = floor((x+1) * GRID_W/2);
		grid[index_y][index_x] = '@';
	}
}



void clearGrid(void)
{
	memset(grid, '.', sizeof(grid));
}


void printGrid(void)
{
	for (int row = 0; row < GRID_H; ++row)
	{
		for (int col = 0; col < GRID_W; ++col)
		{
			char s[3] = { grid[GRID_H - row - 1][col] , ' ', 0 };
			fputs(s, stdout);
		}
		putchar('\n');
	}
	fflush(stdout);
}
