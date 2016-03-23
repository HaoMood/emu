#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 9999            //端口号
#define MAX     10                //最大监听数
#define ADDR "127.0.0.1"    //本地ip地址

//定义字体打印时的颜色
#define LIGHT_BLUE   "\033[1;34m"
#define NONE         "\033[m"

//定义相关变量
int sockfd, newsockfd, is_connected[MAX], fd;
struct sockaddr_in addr;
int addr_len = sizeof(struct sockaddr_in);
fd_set readfd;
char msgbuffer[2048];
//定义连接成功时欢迎信息
char msg[] = LIGHT_BLUE"This is the msg from server. conneceted\nYou can input some cmd like ls, date, cal, w, etc to see the output of the server"NONE;

//初始化服务器
void init_server(void)
{
    //建立socket并错误检查
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket err\n");
        return -1;
    }
    printf("socket created ");
    printf("socked id: %d\n", sockfd);

    //设置端口、ip地址等
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ADDR);

    //绑定端口
    if(bind(sockfd, &addr, sizeof(addr)) < 0)
    {
        printf("bind err\n");
        return -1;
    }
    printf("binded ");
    printf("local port: %d\n", PORT);

    //监听
    if(listen(sockfd, MAX) < 0)
    {
        printf("listen err\n");
        return -1;
    }
    printf("listen\n");

    //清空已连接标记
    for(fd= 0; fd < MAX; ++fd)
    {
        is_connected[fd] = 0;
    }

}

int main(void)
{
    init_server();

    //主循环
    while(1)
    {
        //清空监视集
        FD_ZERO(&readfd);
        FD_SET(sockfd, &readfd);
        //判断有无连接
        for(fd = 0; fd < MAX; ++fd)
        {
            //有连接则设置监视集
            if(is_connected[fd])
            {
                FD_SET(fd, &readfd);
            }
        }
        
        //若达到了最大连接数则直接进入下次循环
        if(!select(MAX, &readfd, NULL, NULL, NULL))
        {
            continue;
        }

        for(fd = 0; fd < MAX; ++fd)
        {
            //判断监听集
            if(FD_ISSET(fd, &readfd))
            {
                //若为sockfd
                if(sockfd == fd)
                {
                    //接受一个新连接
                    if((newsockfd = accept(sockfd, &addr, &addr_len)) < 0)
                    {
                        printf("accept err\n");
                    }
                    //输出欢迎提示信息
                    write(newsockfd, msg, sizeof(msg));
                    //设置标志
                    is_connected[newsockfd] = 1;
                    printf("accept ");
                    printf("connect from %s\n", inet_ntoa(addr.sin_addr));
                }
                else
                {
                    //清空字符串
                    bzero(msgbuffer, sizeof(msgbuffer));
                    //读入用户输入命令
                    if(read(fd, msgbuffer, sizeof(msgbuffer)) <= 0)
                    {
                        //若失败，则关闭连接
                        printf("connect closed\n");
                        is_connected[fd] = 0;

                        close(fd);
                    }
                    else
                    {
                        //将fd重定向到stdout
                        dup2(fd, 1);
                        //调用系统命令
                        system(msgbuffer);
                    }
                }
            }
        }
    }
    return 0;
}

