#include <linux/fb>
extern unsigned char* fbp;
extern struct fb_fix_screeninfo finfo;

//画点
void point(int x, int y, int color)
{
	unsigned int offset;
	if(x >= 800 || x < 0 || y >= 480 || y < 0)
	{
		return;
	}
	//计算偏移量
	offset = y*finfo.line_length + x*4;
	//将颜色信息赋值给内存
	*(unsigned int*)(fbp + offset) = color;
}
