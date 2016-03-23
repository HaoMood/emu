#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/soundcard.h>
#include <pthread.h>
#include "fbtest.h"

#define LENGTH    5	        //存储秒数
#define RATE	44100       //采样频率
#define SIZE    16          //量化位数
#define CHANNELS    2       //声道数目

//用于保存数字音频数据的内存缓冲区
unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8];

//用于将录制好的声音以波形的形式显示在显示屏上
void* print(void* unused)
{
    int x, i;
    //清屏
    clear_screen();
    //将波形显示在显示屏上
    for(x = 0, i = 0; i != (LENGTH*RATE*SIZE*CHANNELS/8); ++x, i += 2)
    {
        //一行显示满后从0重新显示
    	if(x == 800)
    	{
    		x = 0;
    		clear_screen();
    	}
    	point(x, 240+buf[i]/2, 0xffffffff);
        //适当延时
    	usleep(1e3);
    }
}

//声音设备的文件描述符，用于ioctl调用的参数
//系统调用的返回值
int fd, arg, status;

void init_dsp(void)
{
    //打开声音设备
    fd = open("/dev/dsp", O_RDWR);
    if(fd < 0)
    {
    	printf("open dsp failed\n");
    	exit -1;
    }
    
    //设置采样时的量化位数
    arg = SIZE;
    if(ioctl(fd, SOUND_PCM_WRITE_BITS, &arg) < 0)
    {
    	printf("SOUND_PCM_WRITE_BITS failed\n");
    	exit -1;
    }
    if(arg != SIZE)
    {
    	printf("unable to set sample size\n");
    	exit -1;
    }
    
    //设置采样时的声道数目
    arg = CHANNELS;
    if(ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg) < 0)
    {
    	printf("SOUND_PCM_WRITE_CHANNELS failed\n");
    	exit -1;
    }
    if(arg != CHANNELS)
    {
    	printf("unable to set number of channels\n");
    	exit -1;
    }

    //设置采样时的采样频率
    arg = RATE;
    if(ioctl(fd, SOUND_PCM_WRITE_RATE, &arg) < 0)
    {
    	printf("SOUND_PCM_WRITE_RATE failed\n");
    	exit -1;
    }
}

int main(void)
{
    pthread_t thread_id;
    //初始化显示屏
    fbtest();
    //初始化音频设备
    init_dsp();

    //循环，直到按下Control-C
    while(1)
    {
    	printf("say sth\n");
        //录音
    	if(read(fd, buf, sizeof(buf)) != sizeof(buf))
    	{
    		printf("read wrong number of bytes\n");
    	}
    
    	printf("you said\n");
        
        //创建一个线程用于显示波形
    	pthread_create(&thread_id, NULL, &print, NULL);
        
        //回放
    	if(write(fd, buf, sizeof(buf)) != sizeof(buf))
    	{
    		printf("write wrong number of bytes\n");
    	}

        //在继续录音前等待回放结束
    	if(ioctl(fd, SOUND_PCM_SYNC, 0) < 0)
    	{
    		printf("SOUND_PCM_SYNC failed\n");
    	}
    }
    return 0;
}
