#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT	9999
#define REMOTE_IP "127.0.0.1"

int main(void)
{
	int s;
	struct sockaddr_in addr;
	char mybuffer[2048];
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket err\n");
		return -1;
	}
	printf("socket created\n");
	printf("socked id: %d\n", s);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(REMOTE_IP);
	if(connect(s, &addr, sizeof(addr)) < 0)
	{
		printf("connect err\n");
		return -1;
	}
	printf("connected ok!\n");
	printf("remote ip: %s\n", REMOTE_IP);
	printf("remote port: %d\n", PORT);
	recv(s, mybuffer, sizeof(mybuffer), 0);
	printf("%s\n", mybuffer);
	while(1)
	{
		bzero(mybuffer, sizeof(mybuffer));
		read(STDIN_FILENO, mybuffer, sizeof(mybuffer));
		if(send(s, mybuffer, sizeof(mybuffer), 0) < 0)
		{
			printf("send err\n");
			return -1;
		}
		bzero(mybuffer, sizeof(mybuffer));
		recv(s, mybuffer, sizeof(mybuffer), 0);
		printf("%s\n", mybuffer);
	}
	return 0;
}
