#pragma once
#include <math.h>


typedef float radian;
typedef float degree;

#define deg2rad(d) ((degree)(((d) * M_PI) / 180))
#define rad2deg(r) ((radian)(((r) * 180) / M_PI))

#define radRound(r) (radian)(remainder(r, 2*M_PI))
#define degRound(d) (degree)(remainder(d, 360))



typedef union
{
	radian tuple[3];
	struct {
		radian x, y, z;
	};
} AxisRotation;


typedef struct
{
	radian up, left;
} ViewDirection;



AxisRotation* ViewDirection_to_AxisRotation(const ViewDirection* va, AxisRotation* ar);

ViewDirection* ViewDirection_add(ViewDirection* src, const ViewDirection* dst);
ViewDirection* ViewDirection_sub(ViewDirection* src, const ViewDirection* dst);
