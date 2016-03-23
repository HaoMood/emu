#include <stdio.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>


int fd;
unsigned char* fbp;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
int fbsize;

//清屏
inline void clear_screen(void)
{
	//将内存全部赋值为0
	bzero(fbp, fbsize);
}


//设置背景色
void set_bg_color(int color)
{
	int x, y;
	//遍历所有点单元，将其赋值为颜色
	for(x = 0; x < vinfo.xres; ++x)
	{
		for(y = 0; y < vinfo.yres; ++y)
		{
			point(x, y, color);
		}
	}
}

//初始化framebuffer
void fb_init(int color)
{
	//打开设备
	fd = open("/dev/fb", O_RDWR);
	ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);

	//输出缓冲信息
	printf("x = %d, y = %d\n", vinfo.xres, vinfo.yres);
	printf("bits = %d\n", vinfo.bits_per_pixel);

	fbsize = vinfo.yres * finfo.line_length;
	
	printf("screen size = %d\n", fbsize);

	//内存映射
	fbp = (char*)mmap(NULL, fbsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	//清屏
	clear_screen();
	set_bg_color(color);
}

void fb_close(void)
{
	munmap(fbp, fbsize);
	close(fd);
}

