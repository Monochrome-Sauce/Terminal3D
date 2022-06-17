#pragma once
#include "Camera.h"
#include <stdio.h>


typedef Vec4 Polygon[3];

#define DISPLAY_CLR_LINE() printf("\33[2K"); fflush(stdout)
#define DISPLAY_CLR_SCRN() printf("\033[H\033[J"); fflush(stdout)
#define DISPLAY_GOTOXY(x,y) printf("\033[%d;%dH", (y), (x))


void alignVectors(const Camera* cam, Vec4* vecs, size_t nCount);



void renderPoints(const Camera* cam, const Vec4* points, int nCount);

void renderPolygon(const Camera* cam, Polygon poly);



void clearBuffer(void);
void printBuffer(void);
