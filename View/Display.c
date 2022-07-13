#include "../Math/MathLib.h"
#include "ViewPortMath.h"
#include "Display.h"
#include "Camera.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>



char grid[GRID_H][GRID_W] = { 0 };


static int Vec4_cmpz(const Vec4* v1, const Vec4* v2)
{
	return (int)(v1->z - v2->z);
}


static void zsortVec4(Vec4* v, size_t count)
{
	qsort(v, count, sizeof(*v), (__compar_fn_t)Vec4_cmpz);
}




static void plotXY(const int x, const int y, const char c)
{
	if (IN_RANGE(-1, y, GRID_H) && IN_RANGE(-1, x, GRID_W))
	{
		grid[y][x] = c;
	}
}

void plotLine(int x0, int y0, const int x1, const int y1)
{
	int dx = abs(x1 - x0);
	int sx = x1 > x0 ? 1 : -1;
	
	int dy = -abs(y1 - y0);
	int sy = y1 > y0 ? 1 : -1;
	
	int error = dx + dy;
	while (true)
	{
		plotXY(x0, y0, '#');
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






void alignVectors(const Camera* cam, Vec4* vecs, const size_t nCount)
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


void renderPolygon(const Camera* cam, const Polygon polygon)
{
	Mat4x4 x_rotation;
	vec_xRotate_mat(cam->view.up * cos(cam->view.left), x_rotation);
	
	Mat4x4 y_rotation;
	vec_yRotate_mat(cam->view.left, y_rotation);
	
	Mat4x4 z_rotation;
	vec_xRotate_mat(cam->view.up * sin(cam->view.left), z_rotation);
	
	
	struct Coordi coords[3] = {{-1,-1},{-1,-1},{-1,-1}};
	
	
	Polygon poly;
	memcpy(poly, polygon, sizeof(Polygon));
	
	for (int i = 0; i < 3; ++i)
	{
		Vec4* v = &poly[i];
		vec_sub3_vec(v, &cam->position);
		vec_cross_mat(v, x_rotation);
		vec_cross_mat(v, y_rotation);
		vec_cross_mat(v, z_rotation);
	}
	
	zsortVec4(poly, 3);
	const float max_z = poly[2].z;
	const float min_z = poly[0].z;
	
	
	
	
	const float z_limit = 500 / (tan(cam->x_fov/2)*tan(cam->y_fov/2));
	if (!IN_RANGE(0, min_z, z_limit) || !IN_RANGE(0, max_z, z_limit)) return;
	
	for (int i = 0; i < 3; ++i)
	{
		Vec4* v = &poly[i];
		vec_project(v);
		const float x = calcRelPosIn2D(cam->x_fov, v->z, v->x);
		const float y = calcRelPosIn2D(cam->y_fov, v->z, v->y);
		
		coords[i] = (struct Coordi){ .x = widthIndex(x), .y = heightIndex(y) };
	}
	
	for (int i = 0; i < 3; ++i)
	{
		plotLine(coords[i].x, coords[i].y, coords[(i+1)%3].x, coords[(i+1)%3].y);
		if (coords[i].y > 0 && coords[i].y > 0)
		{
			plotXY(coords[i].x, coords[i].y, '@');
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
	fputs("      ╔", stdout);
	fputVerticalBorder(stdout);
	fputs("╗\n", stdout);
	
	for (int row = 0; row < GRID_H; ++row)
	{
		fprintf(stdout, "%+.2f<", 0.99-((row*2.0)/GRID_H));
		fputs("║", stdout);
		for (int col = 0; col < GRID_W; ++col)
		{
			char s[3] = { grid[GRID_H - row - 1][col] , ' ', 0 };
			fputs(s, stdout);
		}
		fputs("║\n", stdout);
	}
	
	fputs("      ╚", stdout);
	fputVerticalBorder(stdout);
	fputs("╝\n", stdout);
	
	fflush(stdout);
}
