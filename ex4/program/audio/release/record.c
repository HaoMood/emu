#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/soundcard.h>
#include <pthread.h>
#include "fbtest.h"

#define LENGTH	5
#define	RATE	44100
#define SIZE	16
#define CHANNELS	2

unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8];

void* print(void* unused)
{
	int x, i;
	clear_screen();
	for(x = 0, i = 0; i != (LENGTH*RATE*SIZE*CHANNELS/8); ++x, i += 2)
	{
		if(x == 800)
		{
			x = 0;
			clear_screen();
		}
		point(x, 240+buf[i]/2, 0xffffffff);
		usleep(1e3);
	}
}

int fd, arg, status;

void init_dsp(void)
{
	fd = open("/dev/dsp", O_RDWR);
	if(fd < 0)
	{
		printf("open dsp failed\n");
		exit -1;
	}
	
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
	fbtest();
	init_dsp();
	while(1)
	{
		printf("say sth\n");
		if(read(fd, buf, sizeof(buf)) != sizeof(buf))
		{
			printf("read wrong number of bytes\n");
		}
	
		printf("you said\n");
		
		pthread_create(&thread_id, NULL, &print, NULL);
		
		if(write(fd, buf, sizeof(buf)) != sizeof(buf))
		{
			printf("write wrong number of bytes\n");
		}

		if(ioctl(fd, SOUND_PCM_SYNC, 0) < 0)
		{
			printf("SOUND_PCM_SYNC failed\n");
		}
	}
	return 0;
}
