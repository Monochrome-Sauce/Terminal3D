#include "Display.h"
#include "../Math/MathLib.h"
#include "../CodeTools.h"

#include <memory.h>
#include <stdio.h>
#include <math.h>



#define GRID_H 60//180
#define GRID_W 120//460

char grid[GRID_H][GRID_W] = { 0 };



static void plotXY(int x, int y, char c)
{
	if (IN_RANGE(-1, y, GRID_H) && IN_RANGE(-1, x, GRID_W))
	{
		grid[y][x] = c;
	}
}


static void plotLineLow(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	
	int yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	
	int D = (2 * dy) - dx;
	int y = y0;
	
	for (int x = x0; x < x1; ++x)
	{
		plotXY(x, y, '=');
		if (D > 0)
		{
			y += yi;
			D += 2 * (dy - dx);
		}
		else
		{
			D += 2*dy;
		}
	}
}

static void plotLineHigh(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	
	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	
	int D = (2 * dx) - dy;
	int x = x0;
	
	for (int y = y0; y < y1; ++y)
	{
		plotXY(x, y, '+');
		if (D > 0)
		{
			x += xi;
			D += 2 * (dx - dy);
		}
		else
		{
			D += 2*dx;
		}
	}
}



static void plotLine(int x0, int y0, int x1, int y1)
{
	//if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0) return;
	
	if (abs(y1 - y0) < abs(x1 - x0))
	{
		if (x0 > x1)
		{
			plotLineLow(x1, y1, x0, y0);
		}
		else
		{
			plotLineLow(x0, y0, x1, y1);
		}
	}
	else
	{
		if (y0 > y1)
		{
			plotLineHigh(x1, y1, x0, y0);
		}
		else
		{
			plotLineHigh(x0, y0, x1, y1);
		}
	}
	
	
	
	/*
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	
	int error = dx + dy;
	while (true)
	{
		//plot(x0, y0)
		if (x0 == x1 && y0 == y1) break;
		
		int e2 = 2 * error;
		if (e2 >= dy)
		{
			if (x0 == x1) break;
			error += dy;
			x0 += sx;
		}
		
		if (e2 <= dx)
		{
			if (y0 == y1) break;
			error += dx;
			y0 += sy;
		}
	}
	*/
}



void renderPoint(const Camera* cam, Vec4 v)
{
	Mat4x4 x_rotation;
	vec_xRotate_mat(cam->view.up * cos(cam->view.left), x_rotation);
	
	Mat4x4 y_rotation;
	vec_yRotate_mat(cam->view.left, y_rotation);
	
	Mat4x4 z_rotation;
	vec_xRotate_mat(cam->view.up * sin(cam->view.left), z_rotation);
	
	float max_z = 20 / (tan(cam->x_fov/2)*tan(cam->y_fov/2));
	
	vec_sub3_vec(&v, &cam->position);
	vec_cross_mat(&v, x_rotation);
	vec_cross_mat(&v, y_rotation);
	vec_cross_mat(&v, z_rotation);
	
	if (IN_RANGE(0, v.z, max_z))
	{
		vec_project(&v);
		float x = v.x/(tan(cam->x_fov/2) * v.z);
		float y = v.y/(tan(cam->y_fov/2) * v.z);
		
		if (IN_RANGE(-1.0, x, +1.0) && IN_RANGE(-1.0, y, +1.0))
		{
			int x_index = floor((x+1)*(GRID_W/2.0));
			int y_index = floor((y+1)*(GRID_H/2.0));
			grid[y_index][x_index] = "@$#%*"[(int)ceil(5*v.z/max_z)-1];
		}
	}
}


void renderPolygon(const Camera* cam, Polygon poly)
{
	Mat4x4 x_rotation;
	vec_xRotate_mat(cam->view.up * cos(cam->view.left), x_rotation);
	
	Mat4x4 y_rotation;
	vec_yRotate_mat(cam->view.left, y_rotation);
	
	Mat4x4 z_rotation;
	vec_xRotate_mat(cam->view.up * sin(cam->view.left), z_rotation);
	
	float max_z = 20 / (tan(cam->x_fov/2)*tan(cam->y_fov/2));
	
	int x_coords[3] = {-1,-1,-1};
	int y_coords[3] = {-1,-1,-1};
	
	for (int i = 0; i < 3; ++i)
	{
		Vec4 v = poly[i];
		vec_sub3_vec(&v, &cam->position);
		vec_cross_mat(&v, x_rotation);
		vec_cross_mat(&v, y_rotation);
		vec_cross_mat(&v, z_rotation);
		
		/*if (IN_RANGE(0, v.z, max_z))
		{
			vec_project(&v);
			float x = v.x/(tan(cam->x_fov/2) * v.z);
			float y = v.y/(tan(cam->y_fov/2) * v.z);
			
			if (IN_RANGE(-1.0, x, +1.0) && IN_RANGE(-1.0, y, +1.0))
			{
				x_coords[i] = floor((x+1)*(GRID_W/2.0));
				y_coords[i] = floor((y+1)*(GRID_H/2.0));
			}
		}*/
		(void)max_z;
		
		vec_project(&v);
		float x = v.x/(tan(cam->x_fov/2) * v.z);
		float y = v.y/(tan(cam->y_fov/2) * v.z);
		
		x_coords[i] = floor((x+1)*(GRID_W/2.0));
		y_coords[i] = floor((y+1)*(GRID_H/2.0));
	}
	
	plotLine(x_coords[0], y_coords[0], x_coords[1], y_coords[1]);
	plotLine(x_coords[1], y_coords[1], x_coords[2], y_coords[2]);
	plotLine(x_coords[0], y_coords[0], x_coords[2], y_coords[2]);
	
	for (int i = 0; i < 3; ++i)
	{
		if (y_coords[i] > 0 && x_coords[i] > 0)
		{
			plotXY(x_coords[i], y_coords[i], '@');
			//"@$#%*"[(int)ceil(5*v.z/max_z)-1];
		}
	}
}




void clearGrid(void)
{
	memset(grid, '.', sizeof(grid));
}


void printGrid(void)
{
	fputs("╔", stdout);
	for (int i = 0; i < (GRID_W*2); ++i)
	{
		fputs("═", stdout);
	}
	fputs("╗\n", stdout);
	
	for (int row = 0; row < GRID_H; ++row)
	{
		fputs("║", stdout);
		for (int col = 0; col < GRID_W; ++col)
		{
			char s[3] = { grid[GRID_H - row - 1][col] , ' ', 0 };
			fputs(s, stdout);
		}
		fputs("║\n", stdout);
	}
	
	fputs("╚", stdout);
	for (int i = 0; i < (GRID_W*2); ++i)
	{
		fputs("═", stdout);
	}
	fputs("╝\n", stdout);
	
	fflush(stdout);
}
