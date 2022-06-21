#pragma once
#include "../CodeTools.h"
#include "../Math/AngleMath.h"


#if 1
#define GRID_H 60
#define GRID_W 120
#else
#define GRID_H 180
#define GRID_W 460
#endif


struct Coordf
{
	float x, y;
};

struct Coordi
{
	int x, y;
};



bool isValidPos(float x, float y);

float calcRelPosIn2D(radian fov, float depth, float posInSpace);

int widthIndex(float relx);

int heightIndex(float rely);



bool findRightIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2);

bool findLeftIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2);

bool findTopIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2);

bool findBottomIntersection(struct Coordf* res, const struct Coordf* p1, const struct Coordf* p2);
