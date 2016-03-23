#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>

//定义8253和8255的各端口地址
#define PB 0x61
#define PORT_CTL 0x63
#define TIMER2 0x42
#define TIMER_CTL 0x43

//打开设备
int drv_open(struct inode* inode, struct file* filp) 
{
    printk("<1>speaker: dev is open\n");
    //先读写低八位，再读写高八位，设置为方式3，采用二进制计数
    //这个控制字即为10110110B，即0xb6
    outb_p(0xb6, TIMER_CTL);
    //PB1=1, PB0=1, 即00000011B, 0x03
    outb_p(3, PB);
    return 0;
}


int drv_release(struct inode* inode, struct file* filp)
{
    //PB1=0, PB0=0, 关闭扬声器
    outb_p(0, PB);
    printk("<1>speaker: dev is closed\n");
    return 0;
}

//先写低八位，再写高八位
int drv_write(struct inode* inode, char* buf, size_t length, loff_t *offset)
{
    outb_p(buf[0], TIMER2);
    outb_p(buf[1], TIMER2);
    return 0;
}

//设备文件操作接口定义
struct file_operations fop =
{
    open : drv_open,
    write : drv_write,
    release : drv_release,
};

//初始化模块
int init_module()
{
    if(register_chrdev(240, "speaker", &fop) < 0)
    {
        printk("speaker: register err\n");
        return -1;
    }
    printk("<1>speaker: register success\n");
    return 0;
}

//模块的卸载
void cleanup_module()
{
    unregister_chrdev(240, "speaker");
    printk("<1>speaker: unregister dev\n");
}

MODULE_LICENSE("GPL");
