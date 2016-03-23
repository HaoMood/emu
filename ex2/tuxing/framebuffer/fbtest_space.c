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

//画点
void point(int x, int y, int color)
{
    unsigned int offset;
    //计算偏移量
    offset = y*finfo.line_length + x*4;
    //将颜色信息赋值给内存
    *(unsigned int*)(fbp + offset) = color;
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
void fbtest(int color)
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

//画线
void draw_line(int x1, int y1, int x2, int y2, int color)
{
    int dx, dy;

    int x, y, i;
    int temp;
    int k, p;
    
    //*************如果x1 > x2, 则交换两坐标点***********
    if(x2 < x1)
    {
        temp = x1;
        x1 = x2;
        x2 = temp;

        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    dx = x2 - x1;
    dy = y2 - y1;

    //******************如果是水平线*********************
    if(dy == 0)
    {
        for(x = x1; x <= x2; ++x)
        {
            point(x, y1, color);
        }
        return;
    }

    //*****************如果是竖直线********************
    if(dx == 0)
    {
        if(y1 < y2)
        {
            for(y = y1; y <= y2; ++y)
            {
                point(x1, y, color);
            }
            return;
        }
        else
        {
            for(y = y2; y <= y1; ++y)
            {
                point(x1, y, color);
            }
            return;
        }
    }

    //****************如果斜率为1********************
    if(dx == dy)
    {
        for(i = 0; i < (x2 - x1); ++i)
        {
            point(x1 + i, y1 + i, color);
        }
        return;
    }

    //****************如果斜率为-1*******************
    if(dx + dy == 0)
    {
        for(i = 0; i < (x2 - x1); ++i)
        {
            point(x1 + i, y1 - i, color);
        }
        return;
    }

    //***************如果斜率小于1, 大于0******************
    if(dy < dx && dy > 0)
    {
        x = x1;
        y = y1;
        point(x1, y1, color);
        p = 2*dy - dx;
        for(i = 0; i < (x2 - x1); ++i)
        {
            if(p < 0)
            {
                point(++x, y, color);
                p += 2*dy; 
            }
            else
            {
                point(++x, ++y, color);
                p += 2*dy - 2*dx;
            }
        }
        return;
    }
    
    if(dy > dx && dy > 0)
    {
        //********如果斜率大于1********************
        x = x1;
        y = y1;
        point(x1, y1, color);
        p = 2*dx - dy;
        for(i = 0; i < (y2 - y1); ++i)
        {
            if(p < 0)
            {
                point(x, ++y, color);
            }
            else
            {
                point(++x, ++y, color);
            }
            p += 2*dx; 
        }
        return;
    }

    //**********如果斜率小于0，大于-1****************
    if((-dy) < dx && dy < 0)
    {
        x = x1;
        y = y1;
        point(x1, y1, color);
        p = 2*(-dy) - dx;
        for(i = 0; i < (x2 - x1); ++i)
        {
            if(p < 0)
            {
                point(++x, y, color);
                p += 2*(-dy); 
            }
            else
            {
                point(++x, --y, color);
                p += 2*(-dy) - 2*dx;
            }
        }
        return;
    }
    if((-dy) > dx && dy < 0)
    {
        //********如果斜率大于1********************
        x = x1;
        y = y1;
        point(x1, y1, color);
        p = 2*dx + dy;
        for(i = 0; i < (y1 - y2); ++i)
        {
            if(p < 0)
            {
                point(x, --y, color);
                p += 2*dx; 
            }
            else
            {
                point(++x, --y, color);
                p += 2*dy - 2*dx;
            }
        }
        return;
    }
}


#define PI 3.1415926
//画圆
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

//画矩形
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

//显示图像
void display_bmp(char* path)
{
    int bmp_fd, x, y;
    char buf[3] = {0};
    //打开图像文件并错误判断
    if((bmp_fd = open(path, O_RDONLY, 0666)) < 0)
    {
        fprintf(stderr, "Cannot open file %s", path);
        exit(0);
    }
    
    //定位文件指针
    lseek(bmp_fd, 54, SEEK_SET);
    //将文件内容复制到缓冲区
    for(y = vinfo.yres - 1; y >= 0; --y)
    {
        for(x = 0; x < vinfo.xres; ++x)
        {
            read(bmp_fd,buf,3);
            memcpy(fbp + 4*(x + y*vinfo.xres), buf, 4);
        }     
    }
    
	//关闭文件描述符
    close(bmp_fd);        
            
}

//输出字符串函数
void printlcd(int x,int y,char *s)
{
    register int i,j,k;
     FILE *fpa,*fph;
     char buffera[16],bufferh[32];
     unsigned char qh,wh;
     unsigned long location;

	 //打开中英文字库
     if((fpa=fopen("./asc16","rb"))==NULL)
         perror("open asc16");
      if((fph=fopen("./hzk16","rb"))==NULL)
        perror("open hzk16");
       while(*s)
    {
       if((char)(*s)>0)
       {
		    //如果是英文字符
            fseek(fpa,(*s)*16,0);
            fread(buffera,1,16,fpa);
            for(i=0;i<16;i++)
                for(j=0;j<8;j++)
                {
                    if(((buffera[i]>>(7-j))&0x01)!=0)
                    point(j+x, i+y, 0xffffffff);
                }
                s++;
                x+=8;
        }
        else
        {
			//如果是中文字符
            qh=*s-0xa0;
            wh=*(s+1)-0xa0;
            location=(94*(qh-1)+(wh-1))*32L;
            fseek(fph,location,SEEK_SET);
            fread(bufferh,32,1,fph);
            for(i=0;i<16;i++)
                for(j=0;j<2;j++)
                    for(k=0;k<8;k++)
                        if(((bufferh[i*2+j]>>(7-k))&0x01)!=0)
                            point(x+8*j+k, y+i, 0xffffffff);
                        s+=2;
                        x+=16;
          }
    }
    fclose(fpa);
    fclose(fph);
}

//碰撞动画函数
void pengzhuang(float* x, float* v, float dt)
{
	//清屏
    clear_screen();
//    printlcd(300, 60, "121180155 zhanghao");
	//画出方盒
	rectangle(100, 100, 700, 380, 0x00ffff00);
    
	//迭代更新每次的位置坐标
    x[0] += v[0] * dt;
    x[1] += v[1] * dt;

	//四个边界碰撞检测
    if(x[0] <= 120 || x[0] >= 680)
    {
        v[0] *= -1;
    }
    if(x[1] <= 120 || x[1] >= 360)
    {
        v[1] *= -1;
    }
	//画圆
    draw_circle((int)(x[0]+0.5), (int)(x[1]+0.5), 25, 0x0000ffff);
}

