#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int fd;
    int i, time[1024];
    //各标准音的频率
    int voice[32] = {0, 131, 147, 165, 175, 196, 220, 247, 0, 0, 0,
                        262, 294, 330, 349, 392, 440, 494, 0, 0, 0,
                        523, 587, 659, 698, 784, 880, 987, 0, 0, 0};
    //一段乐谱，1-7代表低音1-7,11-17代表中音1-7，21-27代表高音1-7
    int song[] = {13, 15, 15, 15, 15, 16, 15, 13, 12, 11, 16, 21, 16, 15, 13, 12, 11, 6, 12, 12, 13, 13, 12, 13, 15, 15, 15, 15, 16, 15, 13, 12, 11, 6, 1, 6, 5, 2, 3, 1, 23, 22, 21, 16, 21};
    //各音符的音调
    float tune[] = {0.25, 0.5, 0.25, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25, 0.75, 0.25, 0.5, 0.25, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25, 0.25, 0.5, 0.5, 0.25, 0.125, 0.125, 0.25, 0.25, 0.5};

    char c;
    long long delay;
    int stop = 0;
    
    //打开设备文件并错误检查
    if((fd = open("/dev/speaker", O_RDWR)) < 0)
    {
        printf("open dev err\n");
        return -1;
    }

    //*********************** test for play a song *******************************
#if 1
    printf("test for play a song\n");
    for(i = 0; i < 45; ++i)
    {
        //计算计数器初值
        time[i] =  1.192e6 / (1.0*voice[song[i]]);
        write(fd, &time[i], 2);
        //根据音调延时 
        usleep(tune[i]*2e6);

        //短间隔
        write(fd, &stop, 2);
        usleep(2e4);
    }
#endif
    
    //******************* test for keyboard input *******************************
#if 0
    printf("test for keyboard input\n");
    printf("character 1 - 7 is for low 1 - 7\n");
    printf("character q - u is for medium 1 - 7\n");
    printf("character a - j is for high 1 - 7\n");
    //使系统不必每次回车确认输入
    system("stty cbreak");
    //每读入一个字符，判断它属于哪一个音
    while((c = getchar()) != 'c')
    {
        switch(c)
        {
                case '1': 
                        i = 1;
                        break;
                case '2':
                        i = 2;
                        break;
                case '3':
                        i = 3;
                        break;
                case '4':
                        i = 4;
                        break;
                case '5':
                        i = 5;
                        break;
                case '6':
                        i = 6;
                        break;
                case '7':
                        i = 7;
                        break;
                case 'q': 
                        i = 11;
                        break;
                case 'w':
                        i = 12;
                        break;
                case 'e':
                        i = 13;
                        break;
                case 'r':
                        i = 14;
                        break;
                case 't':
                        i = 15;
                        break;
                case 'y':
                        i = 16;
                        break;
                case 'u':
                        i = 17;
                        break;
                case 'a': 
                        i = 21;
                        break;
                case 's':
                        i = 22;
                        break;
                case 'd':
                        i = 23;
                        break;
                case 'f':
                        i = 24;
                        break;
                case 'g':
                        i = 25;
                        break;
                case 'h':
                        i = 26;
                        break;
                case 'j':
                        i = 27;
                        break;
        }
        //计算计数器初值
        time[i] =  1.192e6 / (1.0*voice[i]);
        write(fd, &time[i], 2);
        sleep(1);
    }
#endif
    
    close(fd);
    return 0;
}
