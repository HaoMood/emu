#include "fb.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int x0, y0, r, color;
	int i;

	fb_init();

	for(i = 0; i < 5000; ++i)
	{
		x0 = rand() % 800;
		y0 = rand() % 480;
		r = rand() % 800;
		color = rand();
		circle(x0, y0, r, color);
	}

	//rectangle(100, 100, 700, 380, 0x00ffff00);

	fb_close();
	return 0;
}
