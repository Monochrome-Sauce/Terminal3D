#pragma once
#include "Camera.h"
#include <stdio.h>


typedef Vec4 Polygon[3];


#define DISPLAY_CLEAR() printf("\033[H\033[J"); fflush(stdout)
#define DISPLAY_GOTOXY(x,y) printf("\033[%d;%dH", (y), (x)); fflush(stdout)


void renderPoints(const Camera* cam, const Vec4* points, int nCount);

void renderPolygon(const Camera* cam, Polygon poly);



void clearGrid(void);
void printGrid(void);
