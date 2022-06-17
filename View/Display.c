#include "Display.h"
#include "../Math/MathLib.h"
#include "../CodeTools.h"
#include "Camera.h"

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

static void plotLine(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0);
	int sx = x1 > x0 ? 1 : -1;
	
	int dy = -abs(y1 - y0);
	int sy = y1 > y0 ? 1 : -1;
	
	int error = dx + dy;
	while (true)
	{
		plotXY(x0, y0, '*');
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
}



static __inline__ bool isValidPos(float x, float y)
{
	return IN_RANGE(-1.0, x, +1.0) && IN_RANGE(-1.0, y, +1.0);
}

static __inline__ float calcRelPosIn2D(radian fov, float depth, float posInSpace)
{
	return posInSpace/(tan(fov/2) * depth);
}

static __inline__ int widthIndex(float relx)
{
	return floor((relx+1)*(GRID_W/2.0));
}

static __inline__ int heightIndex(float rely)
{
	return floor((rely+1)*(GRID_H/2.0));
}



void alignVectors(const Camera* cam, Vec4* vecs, size_t nCount)
{
	Mat4x4 x_rotation;
	vec_xRotate_mat(cam->view.up * cos(cam->view.left), x_rotation);
	
	Mat4x4 y_rotation;
	vec_yRotate_mat(cam->view.left, y_rotation);
	
	Mat4x4 z_rotation;
	vec_xRotate_mat(cam->view.up * sin(cam->view.left), z_rotation);
	
	for (size_t i = 0; i < nCount; ++i)
	{
		vec_sub3_vec(&vecs[i], &cam->position);
		vec_cross_mat(&vecs[i], x_rotation);
		vec_cross_mat(&vecs[i], y_rotation);
		vec_cross_mat(&vecs[i], z_rotation);
	}
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
		float x = calcRelPosIn2D(cam->x_fov, v.z, v.x);
		float y = calcRelPosIn2D(cam->y_fov, v.z, v.y);
		
		if (isValidPos(x, y))
		{
			grid[heightIndex(y)][widthIndex(x)] = "@$#%"[(int)ceil(4*v.z/max_z)-1];
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
	
	//const float max_z = 20 / (tan(cam->x_fov/2)*tan(cam->y_fov/2));
	
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
			float x = calcRelPosIn2D(cam->x_fov, v.z, v.x);
			float y = calcRelPosIn2D(cam->y_fov, v.z, v.y);
			
			if (isValidPos(x, y))
			{
				x_coords[i] = widthIndex(x);
				y_coords[i] = heightIndex(y);
			}
		}*/
		
		vec_project(&v);
		const float x = calcRelPosIn2D(cam->x_fov, v.z, v.x);
		const float y = calcRelPosIn2D(cam->y_fov, v.z, v.y);
		
		x_coords[i] = widthIndex(x);
		y_coords[i] = heightIndex(y);
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




void clearBuffer(void)
{
	memset(grid, '.', sizeof(grid));
}



static void fputVerticalBorder(FILE* stream)
{
	static const int len = GRID_W * 2;
	for (int i = 0; i < len; ++i)
	{
		fputs("═", stream);
	}
}

void printBuffer(void)
{
	fputs("╔", stdout);
	fputVerticalBorder(stdout);
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
	fputVerticalBorder(stdout);
	fputs("╝\n", stdout);
	
	fflush(stdout);
}
