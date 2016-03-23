#ifndef FBTEST_H
#define FBTEST_H

void fbtest(void);
void point(int x, int y, int color);
void rectangle(int x1, int y1, int x2, int y2, int color);
void display_bmp(char* path);
void printlcd(int x,int y,char *s);
void clear_screen(void);
void pengzhuang(float* x, float* v, float dt);
void draw_circle(int xc, int yc, int r, int color);
void draw_line(int x1, int x2, int y1, int y2, int color);

#endif
