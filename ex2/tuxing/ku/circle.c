#include "fb.h"
void draw_circle(int xc, int yc, int r, int color)
{
	int x, y;
	float p;
	
	x = 0; 
	y = r;
	p = 1.25 - r;
	point(x + xc, y + yc, color);
	point(y + xc, x + yc, color);
	point(x + xc, -y + yc, color);
	point(-y + xc, x + yc, color);
	while(x < y)
	{
		p = (x + 1)*(x + 1) + (y - 0.5)*(y - 0.5) - r*r;
		if(p < 0)
		{
			++x;
			point(x + xc, y + yc, color);
			point(y + xc, x + yc, color);
			point(y + xc, -x + yc, color);
			point(x + xc, -y + yc, color);
			point(-x + xc, -y + yc, color);
			point(-y + xc, -x + yc, color);
			point(-y + xc, x + yc, color);
			point(-x + xc, y + yc, color);

			p += 2*x + 3;
		}
		else
		{
			++x;
			--y;
			point(x + xc, y + yc, color);
			point(y + xc, x + yc, color);
			point(y + xc, -x + yc, color);
			point(x + xc, -y + yc, color);
			point(-x + xc, -y + yc, color);
			point(-y + xc, -x + yc, color);
			point(-y + xc, x + yc, color);
			point(-x + xc, y + yc, color);
			p += 2*(x+1) + 1 - 2*y;
		}

	}
}

