#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int i;
	char c;

	int fd = open("/dev/LED", O_RDWR);

	for(i = 0; i < 8; ++i)
	{
		c = (char)i;
		write(fd, &c, 1);
		sleep(1);
	}

	i = 1;

	while(1)
	{
		c = (char)i;
		write(fd, &i, 1);
		sleep(1);
		i <<= 1 ;
		if(i == 8)
		{
			i = 1;
		}
	}
	
	close(fd);
	return 0;
}	
