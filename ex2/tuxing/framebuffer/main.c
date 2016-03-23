void fbtest(void);
void point(int x, int y, int color);
void circle(int x0, int y0, int r, int color);
void rectangle(int x1, int y1, int x2, int y2, int color);
void display_bmp(char* path);
void printlcd(int x,int y,char *s);
void clear_screen(void);

int main(int argc, char* argv[])
{
	int x;

	fbtest();
	circle(400, 240, 20, 0x0000ffff);
	rectangle(100, 100, 700, 380, 0x00ffff00);
	display_bmp("a1.bmp");
	clear_screen();
	printlcd(30, 30, "hello");
	printlcd(30, 60, "你好");
	return 0;
}
