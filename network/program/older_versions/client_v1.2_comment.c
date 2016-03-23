#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT	9999			//远程端口
#define REMOTE_IP "127.0.0.1"	//远程ip地址

//定义变量
int fd;
struct sockaddr_in addr;
char mybuffer[2048];

//初始化客户机
void init_client(void)
{
	//创建socket并错误检查
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket err\n");
		return -1;
	}
	printf("socket created ");
	printf("socked id: %d\n", fd);

	//设置远程端口和ip地址
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(REMOTE_IP);

	//连接远程服务器
	if(connect(fd, &addr, sizeof(addr)) < 0)
	{
		printf("connect err\n");
		return -1;
	}
	printf("connected ");
	printf("remote ip: %s ", REMOTE_IP);
	printf("remote port: %d\n", PORT);

	//接受远程服务器的输出欢迎信息并显示
	recv(fd, mybuffer, sizeof(mybuffer), 0);
	printf("%s\n", mybuffer);
}

int main(void)
{
	init_client();
	while(1)
	{
		//清空字符串
		bzero(mybuffer, sizeof(mybuffer));
		//读入一个命令
		read(STDIN_FILENO, mybuffer, sizeof(mybuffer));
		//发送信息
		if(send(fd, mybuffer, sizeof(mybuffer), 0) < 0)
		{
			printf("send err\n");
			return -1;
		}
		//清空字符串
		bzero(mybuffer, sizeof(mybuffer));
		//接受并输出信息
		recv(fd, mybuffer, sizeof(mybuffer), 0);
		printf("%s\n", mybuffer);
	}
	return 0;
}
