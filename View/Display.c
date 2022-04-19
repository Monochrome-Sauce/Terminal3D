#include "Display.h"
#include "SpaceObjects.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define GOTO_XY(x,y) printf("\033[%d;%dH", (y), (x)); fflush(stdout)

#define IN_RANGE(low, val, high) ((low)<=(val) && (val)<=(high))



static radianAngle calcAngle2D(int d1, int d2)
{
	radianAngle theta = 0;
	
	if (d2 != 0)
	{
		theta = atan(d1 / (double)d2);
	}
	else if (d1 != 0)
	{
		theta = asin(d1 / abs(d1));
	}
	
	theta += M_PI*(d2 < 0);
	return roundRadians(theta);
}



static Angle3D calcAngle3D(_pIn_ const Point3D* p1, _pIn_ const Point3D* p2)
{
	int xd = p2->x - p1->x;
	int yd = p2->y - p1->y;
	int zd = p2->z - p1->z;
	
	degreeAngle x = 10;
	radianAngle y = x;
	(void)y;
	
	Angle3D a = {
		.xz = calcAngle2D(xd, zd),
		.yz = calcAngle2D(yd, zd),
	};
	return a;
}



const char* Point3D_toStr(_pIn_ const Point3D* x)
{
	static char buffer[64] = { 0 };
	sprintf(buffer, "{%d,%d,%d}", x->x, x->y, x->z);
	return buffer;
}

const char* Angle3D_toStr(_pIn_ const Angle3D* x)
{
	static char buffer[64] = { 0 };
	sprintf(buffer, "{%.3f,%.3f}", rad2deg(x->xz), rad2deg(x->yz));
	return buffer;
}



#define GRID_H 50
#define GRID_W 70

char grid[GRID_H][GRID_W] = { 0 };



void renderPoint(_pIn_ const Camera* cam, _pIn_ const Point3D* point)
{
	//fprintf(stderr, "Camera: %s\n", Point3D_toStr(&cam->position));
	//fprintf(stderr, "Point:  %s\n", Point3D_toStr(point));
	
	Angle3D theta = calcAngle3D(&cam->position, point);
	double point_distance = calcDistance(&cam->position, point);
	//fprintf(stderr, "Angle: %s\n", Angle3D_toStr(&theta));
	//fprintf(stderr, "Distance: %f\n", point_distance);
	
	
	//putc('\n', stderr);
	
	double y_max_offset = point_distance * cos(theta.yz) * tan(cam->field_of_view.yz/2);
	double y_offset = point_distance * sin(theta.yz);
	double y = y_offset / y_max_offset;
	
	double x_max_offset = point_distance * cos(theta.xz) * tan(cam->field_of_view.xz/2);
	double x_offset = point_distance * sin(theta.xz);
	double x = x_offset / x_max_offset;
	
	//fprintf(stderr, "Position: x = %f, y = %f\n", x, y);
	//fprintf(stderr, "x_offset = %f | x_max_offset = %f\n", x_offset, x_max_offset);
	
	if (x_max_offset > 0 && IN_RANGE(-1, x, 1) && IN_RANGE(-1, y, 1))
	{
		double yi = (y+1) * GRID_H/2;
		double xi = (x+1) * GRID_W/2;
		//fprintf(stderr, "Plotted at {%.3f,%.3f}\n", xi, yi);
		grid[(int)floor(yi)][(int)floor(xi)] = '@';
	}
	//fputc('\n', stderr);
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
