#include "fb.h"
void rectangle(int x1, int y1, int x2, int y2, int color)
{
	//x1, y1左上， x2, y2右下
	//x1 < x2, y1 < y2
	int x, y;
	for(x = x1; x < x2; ++x)
	{
		point(x, y1, color);
	}
	for(y = y1; y < y2; ++y)
	{
		point(x2, y, color);
	}
	for(x = x2; x > x1; --x)
	{
		point(x, y2, color);
	}
	for(y = y2; y > y1; --y)
	{
		point(x1, y, color);
	}
}

