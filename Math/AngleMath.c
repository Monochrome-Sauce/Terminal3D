#include "AngleMath.h"



AxisRotation* ViewDirection_to_AxisRotation(const ViewDirection* va, AxisRotation* ar)
{
	ar->x = va->up * fabs(cos(va->left));
	ar->y = va->left;
	ar->z = va->up * fabs(sin(va->left));
	return ar;
}



ViewDirection* ViewDirection_add(ViewDirection* src, const ViewDirection* dst)
{
	src->up = radRound(src->up + dst->up);
	src->left = radRound(src->left + dst->left);
	return src;
}


ViewDirection* ViewDirection_sub(ViewDirection* src, const ViewDirection* dst)
{
	src->up = radRound(src->up - dst->up);
	src->left = radRound(src->left - dst->left);
	return src;
}
