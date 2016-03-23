#ifndef FB_H
#define FB_H
void point(int x, int y, int color);
void rectangle(int x1, int y1, int x2, int y2, int color);
void draw_circle(int xc, int yc, int r, int color);
void fb_close(void);
void fb_init(int color);

#endif
