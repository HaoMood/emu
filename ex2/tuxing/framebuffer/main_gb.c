#include "fbtest.h"	

int main(int argc, char* argv[])
{
	float x[2] = {400, 240};
	float v[2] = {1.3, 1};

	fbtest();

	//*********************** test draw lines **********************
#if 0
	draw_line(100, 100, 400, 100, 0xff0000ff);
	draw_line(100, 150, 100, 400, 0xff00ff00);
	draw_line(150, 150, 400, 250, 0xffff0000);
	draw_line(400, 350, 150, 250, 0xffff0000);
	draw_line(500, 50, 600, 250, 0xff00ffff);
	draw_line(500, 400, 600, 250, 0xffff0000);
	draw_line(500, 440, 700, 380, 0xffffff00);
#endif

	//********************* test draw circle **************************
#if 0
	draw_circle(400, 240, 100, 0xffff0000);	
#endif


	//******************** test for action **************************
	while(1)
	{
		pengzhuang(x, v, 0.5);
	}
	
	//********************** test draw picture **********************
#if 0
	display_bmp("a1.bmp");
#endif
	
	//clear_screen();
	//printlcd(30, 30, "hello");
	//printlcd(30, 60, "ÄãºÃ");
	return 0;
}
